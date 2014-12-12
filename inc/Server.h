#include <netinet/in.h>
#include <string>

using namespace std;

class Server {
    private: 
        struct sockaddr_in stAddr;
        int port;
        int nSocket;
        int queueSize;

        Server();
        void configureAddr();
        void configureSocket();
        void createSocket();
        void bindSocket();
        void assignQueueSize();
    public:
        Server(int port, int queueSize);
};
