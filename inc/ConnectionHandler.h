#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

class ConnectionHandler {
    protected:
        int sck;
    public:
        ConnectionHandler(int sck);
        virtual void handleConnection() = 0;
        virtual ~ConnectionHandler();
};

#endif
