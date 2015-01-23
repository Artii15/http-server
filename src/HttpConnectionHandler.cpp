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
    httpMinor = "1"; // Default http minor (may change during handling request if for example client uses HTTP 1.0)
    res = NULL;
    config = &Config::instance();
    sendResource = false;
    closeConnection = false;

    // max_persistent_connections parameter determines how many times client can connect to server using same socket
    // each connection performed by a client to the same socket requires one "token"
    istringstream ss(config->get("settings", "max_persistent_connections"));
    ss >> connectionTokens;
}

void HttpConnectionHandler::setStandardHeaders() {
    // Setting current date and server name headers
    date = DateTime();
    responseHeaders["Date"] = date.getDate();
    responseHeaders["Server"] = config->get("settings", "name");
}

void HttpConnectionHandler::handleConnection() {
    setStandardHeaders();
    try {
        while(!closeConnection) {
            readRequest();
            removeToken();
            respond();
        }
    }
    catch(logic_error &ex) {
        // This exception occurs when client is inactive for longer than timeout parameter, so just return from method
        return;
    }
    catch(HttpException &ex) {
        reportError(ex); // Send errors associated with not existent resource, wrong permissions etc.
    }
    catch(exception &ex) {
        // If something unexpected went wrong, send 500 status
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
    readConnection();
}

void HttpConnectionHandler::verifyProtocolName() {
    // If client is trying to use protocol other than HTTP, then send 400 status
    const string &protocol = reader.get("protocol");
    if(protocol.substr(0, 4) != "HTTP") {
        throw HttpException("400 Bad Request");
    }
}

void HttpConnectionHandler::verifyVersionMajor() {
    // Only HTTP 1.* allowed
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

    if(httpMinor == "1" && reader.get("host").empty()) { // Host header is required in HTTP 1.1
        throw HttpException("400 Bad Request");
    }
}

void HttpConnectionHandler::normalizeHostHeader() {
    const string &requestedHost = reader.get("host");
    if(requestedHost.empty()) {
        this->host = "default"; // Use default domain if not specified (needed to handle 1.0 Http version)
    }
    else {
        // Searching for colon in host header (part after colon is port nr, so ignore it)
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
    
    // Extract host from absolute url (if client gived absolute url)
    if(url.length() > 7 && url.substr(0, 7) == "http://") {
        unsigned int slashPos = url.find('/', 7);
        this->host = url.substr(7, slashPos - 7);
        this->url = url.substr(slashPos);
    }
    else {
        this->url = url;
    }

    // If url is empty or it's "/" then load default resource (for example index.html)
    if(this->url == "/" || this->url.empty()) {
        this->url = config->get("settings", "default_resource");
    }
}

void HttpConnectionHandler::readConnection() {
    const string& requestedConnection = reader.get("connection");
    if(requestedConnection == "close") {
        closeConnection = true; // Close connection if client sent Connection: close header
    }
    else if(requestedConnection.empty() && httpMinor == "0") {
        closeConnection = true; // Close connection if client uses HTTP 1.0
    }
    else {
        closeConnection = false;
    }
}

void HttpConnectionHandler::respond() {
    const string& method = reader.get("method");
    message = "";
    sendResource = true;

    if(closeConnection) {
        responseHeaders["Connection"] = "close";
    }
    else {
        responseHeaders["Connection"] = "keep-alive";
    }

    // Only GET and HEAD methods supported
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
    performHead(); // Perfrom HEAD method first, (server has to send headers anyway)
    if(sendResource == true) {
        fstream &file = res->getResource();
        file.seekg(0, ios::beg);

        size_t messageSize = res->getSize();
        message.resize(messageSize);
        file.read(&message[0], messageSize); // read whole resource to buffer
    }
}

void HttpConnectionHandler::performHead() {
    const DateTime& modificationDate = res->getModificationDate();
    statusCode = "";

    if(!reader.get("if-modified-since").empty()) {
        DateTime requestedDate = DateTime(reader.get("if-modified-since"));
        if(requestedDate <= date && modificationDate <= requestedDate) {
            statusCode = "304 Not Modified";         
            sendResource = false;
        }
    }
    else if(!reader.get("if-unmodified-since").empty()) {
        DateTime requestedDate = DateTime(reader.get("if-unmodified-since"));
        if(requestedDate <= date && modificationDate > requestedDate) {
            statusCode = "412 Precondition Failed";         
            sendResource = false;
        }
    }

    responseHeaders["Last-Modified"] = modificationDate.getDate();

    if(statusCode.empty()) { // Status code may be set earlier, if for example some errors occur
        statusCode = "200 OK";
    
        const string& type = res->getType();
        if(!type.empty()) {
            responseHeaders["Content-Type"] = type;
        }

        ostringstream ss;
        ss << res->getSize();
        responseHeaders["Content-Length"] = ss.str();
    }
}

// This method sends hardcoded html with error message, used to handle exceptions and inform the client
// that something went wrong
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

void HttpConnectionHandler::removeToken() {
    connectionTokens--;    
    if(connectionTokens <= 0) {
        closeConnection = true;
    }
}
