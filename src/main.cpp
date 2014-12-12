#include "Server.h"
#include <string>
#include <iostream>

#define SERVER_PORT 80
#define QUEUE_SIZE 10

using namespace std;

int main() {
    try {
        Server server(SERVER_PORT, QUEUE_SIZE);
    }
    catch(string &ex) {
        cout << ex << endl; 
    }

    return 0;
}
