#include "ConnectionHandler.h"
#include <unistd.h>

void ConnectionHandler::setSocket(int sck) {
    this->sck = sck;
}

ConnectionHandler::~ConnectionHandler() {
    close(sck);
}
