#include "HttpConnectionHandler.h"
#include <iostream>
#include <unistd.h>

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {}

void HttpConnectionHandler::handleConnection() {
    reader.readHeader(sck);    
}
