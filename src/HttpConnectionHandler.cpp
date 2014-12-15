#include "HttpConnectionHandler.h"
#include <unistd.h>
#include <iostream>
#include <strings.h>
#include <string.h>

#define BUF_SIZE 1000

using namespace std;

HttpConnectionHandler::HttpConnectionHandler(int sck) 
: ConnectionHandler(sck) {}

void HttpConnectionHandler::handleConnection() {
    char buf[BUF_SIZE] = { '\0' };

    while((read(sck, buf, sizeof(buf))) > 0) {
        
    }
    
}
