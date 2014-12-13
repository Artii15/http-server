#include "Server.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>

Server::Server(int port, int queueSize) {
    this->port = port;
    this->queueSize = queueSize;
    initialize();
}

void Server::initialize() {
    configureConnectionHandler();
    configureMutex();
    configureAddr();
    configureSocket();
}

void Server::configureMutex() {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutex_init(&sckMutex, &attr);
}

void Server::configureConnectionHandler() {
    switch(port) {
        case 80:
            connectionHandlerFactory = &Server::setHttpConnectionHandler;
            break;
        default:
            throw runtime_error("Not supported service");
            break;
    }
}

void Server::configureAddr() {
    memset(&stAddr, 0, sizeof(struct sockaddr_in));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(port);
}

void Server::configureSocket() {
    createSocket();
    bindSocket();
    assignQueueSize();
}

void Server::createSocket() {
    nSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(nSocket < 0) {
        throw runtime_error("Couldn't create socket");
    }
    int nFoo = 1;
    setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));
}

void Server::bindSocket() {
    int nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if(nBind < 0) {
        throw runtime_error("Couldn't bind name to the socket");
    }
}

void Server::assignQueueSize() {
    int nListen = listen(nSocket, queueSize);
    if(nListen < 0) {
        throw runtime_error("Couldn't assign queue size");
    }
}

void Server::start() {
    struct sockaddr_in stClientAddr;
    socklen_t nTmp = sizeof(struct sockaddr);

    while(1) {
        pthread_mutex_lock(&sckMutex);

        nClientSocket = accept(nSocket, (struct sockaddr*)&stClientAddr, &nTmp);

        if(nClientSocket < 0) {
            throw runtime_error("Couldn't create conncetion's socket");
        }

        pthread_t threadId;
        pthread_create(&threadId, NULL, &Server::handleConnection, this);

        pthread_mutex_lock(&sckMutex);
        pthread_mutex_unlock(&sckMutex);
    }
}

void* Server::handleConnection(void *arg) {
    Server *context = (Server*)arg;
    int sck = context->nClientSocket;
    pthread_mutex_unlock(&(context->sckMutex));

    ConnectionHandler* handler = context->connectionHandlerFactory(sck);
    handler->handleConnection();

    delete handler;

    return 0;
}

ConnectionHandler* Server::setHttpConnectionHandler(int sck) {
    return new HttpConnectionHandler(sck);
}

Server::~Server() {
    pthread_mutex_destroy(&sckMutex);
    close(nSocket);
}
