#include "HttpConnectionHandler.h"

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {
    reader = new HttpHeaderReader();
}

void HttpConnectionHandler::handleConnection() {
    reader->readHeader(sck);    
}

HttpConnectionHandler::~HttpConnectionHandler() {
    delete reader;
}
