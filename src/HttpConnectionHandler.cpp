#include "HttpConnectionHandler.h"
#include "DateTime.h"
#include "Config.h"
#include <unistd.h>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <iostream>

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {
    httpMinor = "1"; 
    res = NULL;
    config = &Config::instance();
    sendResource = false;

    setStandardHeaders();
}

void HttpConnectionHandler::setStandardHeaders() {
    DateTime date;
    responseHeaders["Date"] = date.getDate();
    responseHeaders["Server"] = config->get("settings", "name");
    responseHeaders["Connection"] = "close";
}

void HttpConnectionHandler::handleConnection() {
    try {
        readRequest();
        respond();
    }
    catch(HttpException &ex) {
        reportError(ex);
    }
    catch(exception &ex) {
        HttpException criticalEx("500 Internal Server Error");
        reportError(criticalEx);
    }
}

void HttpConnectionHandler::readRequest() {
    reader.readHeader(sck);    
    verifyProtocolName();
    verifyVersionMajor();
    readVersionMinor();
    normalizeHostHeader();
    readUrl();
}

void HttpConnectionHandler::verifyProtocolName() {
    const string &protocol = reader.get("protocol");
    if(protocol.substr(0, 4) != "HTTP") {
        throw HttpException("400 Bad Request");
    }
}

void HttpConnectionHandler::verifyVersionMajor() {
    const string &protocol = reader.get("protocol");
    unsigned int protocolLen = protocol.length();

    unsigned int i = 5;
    string major = "";
    while(i < protocolLen && protocol[i] != '.') {
        major += protocol[i++];
    }
    i++;

    if(major != "1") {
        throw HttpException("501 Not Implemented");
    }
}

void HttpConnectionHandler::readVersionMinor() {
    const string &protocol = reader.get("protocol");
    unsigned int protocolLen = protocol.length();

    httpMinor = "";
    for(unsigned int i = 7; i < protocolLen; i++) {
        httpMinor += protocol[i];
    }

    if(httpMinor == "1" && reader.get("host").empty()) {
        throw HttpException("400 Bad Request");
    }
}

void HttpConnectionHandler::normalizeHostHeader() {
    const string &requestedHost = reader.get("host");
    if(requestedHost.empty()) {
        this->host = "default";
    }
    else {
        unsigned int colonPos = requestedHost.find(':');
        if(colonPos != string::npos) {
            this->host = requestedHost.substr(0, colonPos);
        }
        else {
            this->host = requestedHost;
        }
    }
}

void HttpConnectionHandler::readUrl() {
    const string& url = reader.get("url"); 
    
    if(url.length() > 7 && url.substr(0, 7) == "http://") {
        unsigned int slashPos = url.find('/', 7);
        this->host = url.substr(7, slashPos - 7);
        this->url = url.substr(slashPos);
    }
    else {
        this->url = url;
    }

    if(this->url == "/" || this->url.empty()) {
        this->url = config->get("settings", "default_resource");
    }
}

void HttpConnectionHandler::respond() {
    const string& method = reader.get("method");
    message = "";
    sendResource = true;

    if(method == "GET") {
        res = new Resource(config->get("domains", host), url);
        performGet();
        delete res;
    }
    else if(method == "HEAD") {
        sendResource = false;
        res = new Resource(config->get("domains", host), url);
        performHead();
        delete res;
    }
    else {
        throw HttpException("501 Not Implemented");
    }

    send();
}

void HttpConnectionHandler::performGet() {
    performHead();
    if(sendResource == true) {
        fstream &file = res->getResource();
        file.seekg(0, ios::beg);

        size_t messageSize = res->getSize();
        message.resize(messageSize);
        file.read(&message[0], messageSize);
    }
}

void HttpConnectionHandler::performHead() {
    statusCode = "200 OK";

    const DateTime& modificationDate = res->getModificationDate();
    responseHeaders["Last-Modified"] = modificationDate.getDate();
    
    const string& type = res->getType();
    if(!type.empty()) {
        responseHeaders["Content-Type"] = type;
    }

    if(!reader.get("if-modified-since").empty()) {
        DateTime requestedDate = DateTime(reader.get("if-modified-since"));
    }
    else if(!reader.get("if-unmodified-since").empty()) {
        DateTime requestedDate = DateTime(reader.get("if-unmodified-since"));
    }

    ostringstream ss;
    ss << res->getSize();
    responseHeaders["Content-Length"] = ss.str();
}

void HttpConnectionHandler::reportError(const HttpException &ex) {
    ostringstream ss;
    statusCode = ex.what();

    ss.str("");

    ss << "<html><head></head><body><h1>" << ex.what() << "</h1></body></html>";
    message = ss.str();

    ss.str("");
    ss << message.length();

    responseHeaders["Content-Length"] = ss.str();
    responseHeaders["Content-Type"] = "text/html";
    responseHeaders["Connection"] = "close";

    send();
}

void HttpConnectionHandler::send() {
    sendStatus();
    sendHeaders();
    sendSeparator();
    if(sendResource) {
        sendMessage();
    }
}

void HttpConnectionHandler::sendStatus() {
    string statusLine = "HTTP/1." + httpMinor + " " + statusCode + "\r\n";
    size_t statusLen = statusLine.length();
    unsigned int sent = 0;

    for(unsigned int toSend = statusLen; toSend > 0; toSend -= sent) {
        sent = write(sck, statusLine.c_str(), statusLen);
    }
}

void HttpConnectionHandler::sendHeaders() {
    boost::unordered_map<string, string>::iterator headersIt = responseHeaders.begin();

    for(; headersIt != responseHeaders.end(); headersIt++) {
        ostringstream ss;
        ss << headersIt->first << ": " << headersIt->second << "\r\n";

        string header = ss.str();
        size_t headerLen = header.length();
        unsigned int sent = 0;

        for(unsigned int toSend = headerLen; toSend > 0; toSend -= sent) {
            sent = write(sck, header.c_str(), headerLen); 
        }
    }
}

void HttpConnectionHandler::sendSeparator() {
    char separator[3] = "\r\n";
    size_t sepLen = sizeof(separator) - 1;
    unsigned int sent = 0;

    for(unsigned int toSend = sepLen; toSend > 0; toSend -= sent) {
        sent = write(sck, separator, sepLen);
    }
}

void HttpConnectionHandler::sendMessage() {
    size_t messageLen = message.length();
    unsigned int sent = 0;

    for(unsigned int toSend = messageLen; toSend > 0; toSend -= sent) {
        sent = write(sck, message.c_str(), messageLen);
    }
}
