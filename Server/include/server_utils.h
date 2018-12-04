#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
using namespace std;

class Server_Utils{
    public: 
        Server_Utils(int port);
        void run_server();
    private: 
        string hostname;
        int sockfd, newsockfd, portno;
        socklen_t clilen;
        struct sockaddr_in serv_addr, cli_addr;
        char buffer[256];
        void error(string msg);
};

#endif