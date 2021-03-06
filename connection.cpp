#include <sys/select.h>
#include "connection.h"
#include <algorithm>
extern int interruptFd;
Connection::Connection() {
    int opt = 1;
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        printf("Socket failed\n");
        return;
    }
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                &opt, sizeof(opt))) {
        printf("setsockopt error\n");
        return;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(7272);
    
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        printf("bind failed\n");
        return;
    }
    if(listen(server_fd, 3) < 0) {
        printf("listen error\n");
        return;
    }
}
Connection::~Connection() {
    close(server_fd);
}

bool Connection::connect() {
    int addrlen = sizeof(address);
    fd_set set;
    FD_ZERO(&set);
    FD_SET(server_fd, &set);
    FD_SET(interruptFd, &set);
    if ((select(std::max(server_fd, interruptFd) + 1, &set, NULL, NULL, NULL)) < 0 || FD_ISSET(interruptFd, &set))
    {
        // printf("interrupt on connect %i\n", FD_ISSET(interruptFd, &set));
        return false;
    }
    if((sock = accept(server_fd, (struct sockaddr*)&address,
                    (socklen_t*)&addrlen)) < 0) {
        printf("accept error\n");
        return false;
    }
    printf("connection accepted\n");
    return true;
}

bool Connection::getData() {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(sock, &set);
    FD_SET(interruptFd, &set);
    if ((select(std::max(sock, interruptFd) + 1, &set, NULL, NULL, NULL)) < 0 || FD_ISSET(interruptFd, &set))
    {
        printf("interrupt on read\n");
        return false;
    }
    int result = read(sock, buffer, 256);
    return result > 0;
}
void* Connection::bufferAddr() {
    return (void*)buffer;
}
