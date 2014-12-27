#include "Server.h"
#include "Config.h"
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <stdexcept>

using namespace std;

Server *server = NULL;

void loadConfig();
void initialize();
void startServer();
void stopServer(int signum);

int main() {
    loadConfig();
    initialize();
    signal(SIGINT, stopServer);
    startServer();

    exit(0);
}

void loadConfig() {
    try {
        Config &config = Config::instance();
        config.load("config/domains", "domains");
        config.load("config/types", "types");
        config.load("config/settings", "settings");
    }
    catch(exception &ex) {
        cout << ex.what() << endl; 
        exit(1);
    }
}

void initialize() {
    try {
        server = new Server();
    }
    catch(exception &ex) {
        cout << ex.what() << endl; 
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
