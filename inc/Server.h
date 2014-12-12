#include <netinet/in.h>
#include <arpa/inet.h>

class Server {
    private: 
        struct sockaddr_in stAddr;
        int port;

        Server();
        void configureAddr();
    public:
        Server(int port);
};
