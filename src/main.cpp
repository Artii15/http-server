#include "Server.h"
#include "Config.h"
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <stdexcept>

#define SERVER_PORT 80
#define QUEUE_SIZE 10

using namespace std;

Server *server = NULL;

void initialize();
void loadConfig();
void startServer();
void stopServer(int signum);

int main() {
    initialize();
    loadConfig();
    signal(SIGINT, stopServer);
    startServer();

    exit(0);
}

void initialize() {
    try {
        server = new Server(SERVER_PORT, QUEUE_SIZE);
    }
    catch(exception &ex) {
        cout << ex.what() << endl; 
        exit(1);
    }
}

void loadConfig() {
    try {
        Config config = Config::instance();
        config.load("config/domains", "domains");
        config.load("config/types", "types");
    }
    catch(exception &ex) {
        cout << ex.what() << endl; 
        delete server;
        exit(1);
    }
}

void startServer() {
    try {
        server->start();
    }
    catch(exception &ex) {
        cout << ex.what() << endl; 
        delete server;
        exit(1);
    }
}

void stopServer(int signum) {
    delete server;
    exit(0);
}
