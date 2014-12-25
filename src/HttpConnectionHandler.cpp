#include "HttpConnectionHandler.h"
#include "HttpException.h"
#include <iostream>
#include <unistd.h>

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {}

void HttpConnectionHandler::handleConnection() {
    readRequest();
    respond();
}

void HttpConnectionHandler::readRequest() {
    reader.readHeader(sck);    
    readProtocol(reader.get("protocol"));
}

void HttpConnectionHandler::readProtocol(const string &protocol) {
    unsigned int protocolLen = protocol.length();
    if(protocol.substr(0, 4) != "HTTP") {
        throw HttpException(400, "Bad Request");
    }

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
}

void HttpConnectionHandler::respond() {

}
