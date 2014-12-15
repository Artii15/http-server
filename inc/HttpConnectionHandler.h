#ifndef HTTP_CONNECTION_HANDLER_H
#define HTTP_CONNECTION_HANDLER_H

#include "ConnectionHandler.h"
#include <list>
#include <string>

using namespace std;

class HttpConnectionHandler: public ConnectionHandler {
    private:
        list<string> headerLines; 
    public:
        HttpConnectionHandler(int sck);
        void handleConnection();
};

#endif
