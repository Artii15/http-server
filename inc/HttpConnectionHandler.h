#ifndef HTTP_CONNECTION_HANDLER_H
#define HTTP_CONNECTION_HANDLER_H

#include "ConnectionHandler.h"

class HttpConnectionHandler: public ConnectionHandler {
    public:
        void handleConnection();
};

#endif