#include "HttpConnectionHandler.h"
#include "HttpException.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {
    httpMinor = 1; 
}

void HttpConnectionHandler::handleConnection() {
    readRequest();
    respond();
}

void HttpConnectionHandler::readRequest() {
    reader.readHeader(sck);    
    readProtocol();
}

void HttpConnectionHandler::readProtocol() {
    verifyProtocolName();
    readHttpVersion();
}

void HttpConnectionHandler::verifyProtocolName() {
    const string &protocol = reader.get("protocol");
    if(protocol.substr(0, 4) != "HTTP") {
        throw HttpException(400, "Bad Request");
    }
}

void HttpConnectionHandler::readHttpVersion() {
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

    string minor = "";
    while(i < protocolLen) {
        minor += protocol[i++];
    }
    this->httpMinor = atoi(minor.c_str());
}

void HttpConnectionHandler::respond() {

}
