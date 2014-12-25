#include "HttpConnectionHandler.h"
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
    
}

void HttpConnectionHandler::respond() {

}
