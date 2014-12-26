#include "HttpConnectionHandler.h"
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <stdexcept>

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {
    httpMinor = 1; 
    statusLine = "";
    message = "";
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
        throw HttpException(500, "Internal Server Error");  
    }
}

void HttpConnectionHandler::readRequest() {
    reader.readHeader(sck);    
    verifyProtocolName();
    verifyVersionMajor();
    readVersionMinor();
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
    iss >> this->httpMinor;

    if(this->httpMinor < 0) {
        throw HttpException(400, "Bad Request");
    }
}

void HttpConnectionHandler::respond() {
    /*
    prepareResource(); 
    sendHeaders();
    if(reader.get("method") == "GET") {
        sendResource();
    }*/
}

void HttpConnectionHandler::reportError(const HttpException &ex) {
    ostringstream ss;
    ss << "HTTP/1." << httpMinor << " " << ex.getCode() << " " << ex.getMessage() << "\r\n";

    statusLine = ss.str();
    ss.clear();

    ss << "<html><head></head><body><h1>" << ex.getCode() << " " << ex.getMessage() << "</h1></body></html>";
    message = ss.str();

    ss.clear();
    ss << message.length();

    responseHeaders["Content-Length"] = ss.str();
    responseHeaders["Content-Type"] = "text/html";
    responseHeaders["Connection"] = "close";

    send();
}

void HttpConnectionHandler::send() {
    sendHeaders();
    sendMessage();
}

void HttpConnectionHandler::sendHeaders() {
    
}

void HttpConnectionHandler::sendMessage() {
    size_t messageLen = message.length();
    unsigned int sent = 0;

    for(unsigned int toSend = messageLen; toSend > 0; toSend -= sent) {
        sent = write(sck, message.c_str(), messageLen);
    }
}
