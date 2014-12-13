#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <pthread.h>
#include "HttpConnectionHandler.h"

using namespace std;

class Server {
    private: 
        struct sockaddr_in stAddr;
        int port;
        int queueSize;
        int nSocket;
        int nClientSocket; 
        pthread_mutex_t sckMutex;
        ConnectionHandler* (*connectionHandlerFactory)(int);

        void initialize();
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
        Server(int port, int queueSize);
        void start();
        ~Server();
};

#endif
