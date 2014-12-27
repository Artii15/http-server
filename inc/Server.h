#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include "HttpConnectionHandler.h"
#include "Config.h"

class Server {
    private: 
        Config *config;
        struct sockaddr_in stAddr;
        int port;
        int queueSize;
        int nSocket;
        int nClientSocket; 
        std::string service;
        pthread_mutex_t sckMutex;
        ConnectionHandler* (*connectionHandlerFactory)(int);

        void readSettings();
        void configureConnectionHandler();
        void configureMutex();
        void configureAddr();
        void configureSocket();
        void createSocket();
        void bindSocket();
        void assignQueueSize();
        static void* handleConnection(void *arg);
        static ConnectionHandler* setHttpConnectionHandler(int sck);
    public:
        Server();
        void start();
        ~Server();
};

#endif
