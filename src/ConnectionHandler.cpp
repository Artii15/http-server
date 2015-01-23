#include "ConnectionHandler.h"
#include <unistd.h>

// This abstract class implements only methods common for potentially all handlers

ConnectionHandler::ConnectionHandler(int sck) {
    this->sck = sck;
}

ConnectionHandler::~ConnectionHandler() {
    close(sck);
}
