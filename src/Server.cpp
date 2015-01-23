#include "../inc/Server.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <sstream>

using namespace std;

Server::Server() {
    readSettings(); // reading info from config/settings
    configureConnectionHandler(); // choosing concrete handler based on service type
    configureMutex(); // mutex initialization 
    configureAddr(); // filling sockaddr struct
    configureSocket(); // Socket initalization (creating and binding, setting queue size)
}

void Server::readSettings() {
    config = &Config::instance();
    service = config->get("settings", "service");

    istringstream ss(config->get("settings", "port"));
    ss >> port;

    ss.str(config->get("settings", "queue_size"));
    ss >> queueSize;
}

void Server::configureMutex() {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutex_init(&sckMutex, &attr);
}

void Server::configureConnectionHandler() {
    // Only http supported now
    if(service == "http") {
        connectionHandlerFactory = &Server::setHttpConnectionHandler;
    }
    else {
        throw runtime_error("Not supported service");
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

    while(1) { // Main server loop
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
    int sck = context->nClientSocket; // copying newly connected client sck, before server accepts another client
    pthread_mutex_unlock(&(context->sckMutex));

    ConnectionHandler* handler = context->connectionHandlerFactory(sck);
    handler->handleConnection();
    delete handler;

    return 0;
}

ConnectionHandler* Server::setHttpConnectionHandler(int sck) {
    // Service delagated to another object
    return new HttpConnectionHandler(sck);
}

Server::~Server() {
    pthread_mutex_destroy(&sckMutex);
    close(nSocket);
}
