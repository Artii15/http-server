#include "HttpConnectionHandler.h"
#include <iostream>

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {}

void HttpConnectionHandler::handleConnection() {
    std::cout << sck << std::endl;
}
