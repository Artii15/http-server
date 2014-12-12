#include "Server.h"
#include <string.h>

Server::Server(int port) {
    this->port = port;
    Server();
}

Server::Server() {
    configureAddr();
}

void Server::configureAddr() {
    memset(&stAddr, 0, sizeof(struct sockaddr_in));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(port);
}
