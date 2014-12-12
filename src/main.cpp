#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SERVER_PORT 80
#define QUEUE_SIZE 10

int main() {
    struct sockaddr_in stAddr;

    memset(&stAddr, 0, sizeof(struct sockaddr_in));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(SERVER_PORT);

    int nSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(nSocket < 0) {
        fprintf(stderr, "Couldn't create socket\n");
        exit(1);
    }
    int nFoo = 1;
    setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

    int nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if(nBind < 0) {
        fprintf(stderr, "Couldn't bind a name to a socket\n");
        exit(1);
    }

    int nListen = listen(nSocket, QUEUE_SIZE);
    if(nListen < 0) {
        fprintf(stderr, "Couldn't set queue size\n");
        exit(1);
    }

    close(nSocket);
    return 0;
}
