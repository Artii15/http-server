#include "HttpConnectionHandler.h"
#include <unistd.h>
#include <iostream>
#include <strings.h>
#include <string.h>

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {
    reader = new HttpHeaderReader(sck);
}

void HttpConnectionHandler::handleConnection() {
    reader->readHeader();    
}

HttpConnectionHandler::~HttpConnectionHandler() {
    delete reader;
}
