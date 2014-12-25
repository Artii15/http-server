#include "HttpConnectionHandler.h"
#include "HttpException.h"
#include <iostream>
#include <unistd.h>
#include <sstream>

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

}
