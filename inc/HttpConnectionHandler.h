#ifndef HTTP_CONNECTION_HANDLER_H
#define HTTP_CONNECTION_HANDLER_H

#include "ConnectionHandler.h"
#include "HttpHeaderReader.h"
#include "HttpException.h"
#include "Resource.h"
#include "Config.h"
#include <string>
#include <boost/unordered_map.hpp>

class HttpConnectionHandler: public ConnectionHandler {
    private:
        HttpHeaderReader reader;
        boost::unordered_map<std::string, std::string> responseHeaders;
        std::string httpMinor;
        std::string message;
        std::string host;
        std::string url;
        Resource *res;
        Config *config;
        std::string statusCode;

        void setStandardHeaders();
        void readRequest();
        void verifyProtocolName();
        void verifyVersionMajor();
        void readVersionMinor();
        void normalizeHostHeader();
        void readUrl();

        void respond();
        void performHead();
        void performGet();
        void reportError(const HttpException &ex);

        void send();
        void sendStatus();
        void sendHeaders();
        void sendSeparator();
        void sendMessage();
    public:
        HttpConnectionHandler(int sck);
        void handleConnection();
};

#endif
