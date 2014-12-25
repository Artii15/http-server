#ifndef HTTP_CONNECTION_HANDLER_H
#define HTTP_CONNECTION_HANDLER_H

#include "ConnectionHandler.h"
#include "HttpHeaderReader.h"
#include <list>
#include <string>

class HttpConnectionHandler: public ConnectionHandler {
    private:
        HttpHeaderReader reader;
        short httpMinor;

        void readRequest();
        void verifyProtocolName();
        void verifyVersionMajor();
        void readVersionMinor();
        void respond();
    public:
        HttpConnectionHandler(int sck);
        void handleConnection();
};

#endif
