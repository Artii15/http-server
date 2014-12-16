#include "HttpConnectionHandler.h"
#include <iostream>

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {
    reader = new HttpHeaderReader();
}

void HttpConnectionHandler::handleConnection() {
    reader->readHeader(sck);    

    cout << reader->get("method") << endl;
    cout << reader->get("route") << endl;
    cout << reader->get("protocol") << endl;
    cout << reader->get("User-Agent") << endl;
}

HttpConnectionHandler::~HttpConnectionHandler() {
    delete reader;
}
