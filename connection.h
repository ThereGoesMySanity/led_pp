#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
class Connection {
    public:
        Connection();
        bool connect();
        bool getData();
        void* bufferAddr();
    private:
        int sock, server_fd;
        struct sockaddr_in address;
        char buffer[256] = { 0 };
};
