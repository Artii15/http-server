#ifndef HTTP_CONNECTION_HANDLER_H
#define HTTP_CONNECTION_HANDLER_H

#include "ConnectionHandler.h"
#include "HttpHeaderReader.h"
#include <list>
#include <string>

class HttpConnectionHandler: public ConnectionHandler {
    private:
        HttpHeaderReader reader;

        void readRequest();
        void readProtocol(const std::string &protocol);
        void respond();
    public:
        HttpConnectionHandler(int sck);
        void handleConnection();
};

#endif
