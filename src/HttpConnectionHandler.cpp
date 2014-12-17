#include "HttpConnectionHandler.h"
#include <iostream>
#include <unistd.h>

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {
    reader = new HttpHeaderReader();
}

void HttpConnectionHandler::handleConnection() {
    reader->readHeader(sck);    
    
    const char* response = "HTTP/1.1 200 OK\r\n\r\nResponse-status: OK\r\n"; 
    
    size_t toSend = 40;
    size_t sent = 0;
    for(; toSend > 0; toSend -= sent){
        sent = write(sck, response, toSend);
    }
}

HttpConnectionHandler::~HttpConnectionHandler() {
    delete reader;
}
