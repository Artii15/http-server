#ifndef HTTP_CONNECTION_HANDLER_H
#define HTTP_CONNECTION_HANDLER_H

#include "../inc/ConnectionHandler.h"
#include "../inc/HttpHeaderReader.h"
#include "../inc/HttpException.h"
#include "../inc/Resource.h"
#include "../inc/Config.h"
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
        DateTime date;
        std::string statusCode;
        bool sendResource;
        bool closeConnection;
        unsigned int connectionTokens;

        void setStandardHeaders();
        void readRequest();
        void verifyProtocolName();
        void verifyVersionMajor();
        void readVersionMinor();
        void normalizeHostHeader();
        void readUrl();
        void readConnection();

        void removeToken();

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
