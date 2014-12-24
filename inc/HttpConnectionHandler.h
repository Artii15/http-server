#ifndef HTTP_CONNECTION_HANDLER_H
#define HTTP_CONNECTION_HANDLER_H

#include "ConnectionHandler.h"
#include "HttpHeaderReader.h"
#include <list>
#include <string>

class HttpConnectionHandler: public ConnectionHandler {
    private:
        HttpHeaderReader reader;
    public:
        HttpConnectionHandler(int sck);
        void handleConnection();
};

#endif
