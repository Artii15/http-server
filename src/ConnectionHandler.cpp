#include "ConnectionHandler.h"
#include <unistd.h>

ConnectionHandler::ConnectionHandler(int sck) {
    this->sck = sck;
}

ConnectionHandler::~ConnectionHandler() {
    close(sck);
}
