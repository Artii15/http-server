#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

using namespace std;

class Server {
    private: 
        struct sockaddr_in stAddr;
        int port;
        int queueSize;
        int nSocket;

        void initialize();
        void configureAddr();
        void configureSocket();
        void createSocket();
        void bindSocket();
        void assignQueueSize();
        void handleConnection(int sockFd);
    public:
        Server(int port, int queueSize);
        void start();
        ~Server();
};

#endif
