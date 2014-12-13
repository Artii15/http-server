#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <pthread.h>

using namespace std;

class Server {
    private: 
        struct sockaddr_in stAddr;
        int port;
        int queueSize;
        int nSocket;
        pthread_mutex_t sockMutex;

        void initialize();
        void initializeMutex();
        void configureAddr();
        void configureSocket();
        void createSocket();
        void bindSocket();
        void assignQueueSize();
        static void* handleConnection(void *arg);
    public:
        Server(int port, int queueSize);
        void start();
        ~Server();
};

#endif
