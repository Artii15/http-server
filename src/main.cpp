#include "Server.h"
#include <iostream>
#include <stdexcept>

#define SERVER_PORT 80
#define QUEUE_SIZE 10

using namespace std;

int main() {
    try {
        Server server(SERVER_PORT, QUEUE_SIZE);
        server.start();
    }
    catch(exception &ex) {
        cout << ex.what() << endl; 
    }

    return 0;
}
