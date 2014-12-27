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
    httpMinor = 1; 
    statusLine = "";
    message = "";
    host = "";
    res = NULL;
    config = &Config::instance();

    setStandardHeaders();
}

void HttpConnectionHandler::setStandardHeaders() {
    DateTime date;
    responseHeaders["Date"] = date.getDate();
    responseHeaders["Server"] = config->get("settings", "name");
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
        HttpException criticalEx(500, "Internal Server Error");
        reportError(criticalEx);
    }
}

void HttpConnectionHandler::readRequest() {
    reader.readHeader(sck);    
    verifyProtocolName();
    verifyVersionMajor();
    readVersionMinor();
    normalizeHostHeader();
}

void HttpConnectionHandler::verifyProtocolName() {
    const string &protocol = reader.get("protocol");
    if(protocol.substr(0, 4) != "HTTP") {
        throw HttpException(400, "Bad Request");
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
        throw HttpException(501, "Not Implemented");
    }
}

void HttpConnectionHandler::readVersionMinor() {
    const string &protocol = reader.get("protocol");
    unsigned int protocolLen = protocol.length();

    string minor = "";
    for(unsigned int i = 7; i < protocolLen; i++) {
        minor += protocol[i];
    }
    istringstream iss(minor);
    iss >> httpMinor;

    if(httpMinor < 0) {
        throw HttpException(400, "Bad Request");
    }

    if(httpMinor == 1 && reader.get("host") == "") {
        throw HttpException(400, "Bad Request");
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

void HttpConnectionHandler::respond() {
    const string& method = reader.get("method");
    string& url = reader.get("url");
    if(url == "/") {
        url = "index.html";
    }

    if(method == "GET") {
        res = new Resource(config->get("domains", host), url);
        performGet();
        delete res;
    }
    else if(method == "HEAD") {
        res = new Resource(config->get("domains", host), url);
        performHead();
        delete res;
    }
    else {
        throw HttpException(501, "Not Implemented");
    }

    send();
}

void HttpConnectionHandler::performGet() {
    performHead();
    
    fstream &file = res->getResource();
    file.seekg(0, ios::beg);

    size_t messageSize = res->getSize();
    message.resize(messageSize);
    file.read(&message[0], messageSize);
}

void HttpConnectionHandler::performHead() {
    ostringstream ss;
    ss << "HTTP/1." << httpMinor << " 200 OK";
    statusLine = ss.str();
    ss.str("");
    
    const string& type = res->getType();
    if(type != "") {
        responseHeaders["Content-Type"] = type;
    }

    ssize_t size = res->getSize();
    if(size >= 0) {
        ss << size;
        responseHeaders["Content-Length"] = ss.str();
    }

    message = "";
}

void HttpConnectionHandler::reportError(const HttpException &ex) {
    ostringstream ss;
    ss << "HTTP/1." << httpMinor << " " << ex.getCode() << " " << ex.getMessage();

    statusLine = ss.str();
    ss.str("");

    ss << "<html><head></head><body><h1>" << ex.getCode() << " " << ex.getMessage() << "</h1></body></html>";
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
    sendMessage();
}

void HttpConnectionHandler::sendStatus() {
    statusLine += "\r\n";
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
