#ifndef HTTP_CONNECTION_HANDLER_H
#define HTTP_CONNECTION_HANDLER_H

#include "ConnectionHandler.h"
#include "HttpHeaderReader.h"
#include "HttpException.h"
#include <string>
#include <boost/unordered_map.hpp>

class HttpConnectionHandler: public ConnectionHandler {
    private:
        HttpHeaderReader reader;
        short httpMinor;
        boost::unordered_map<std::string, std::string> responseHeaders;
        std::string statusLine;
        std::string message;

        void readRequest();
        void verifyProtocolName();
        void verifyVersionMajor();
        void readVersionMinor();
        void respond();
        void reportError(const HttpException &ex);
        void send();
        void sendHeaders();
        void sendMessage();
    public:
        HttpConnectionHandler(int sck);
        void handleConnection();
};

#endif
