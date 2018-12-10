#ifndef CLIENT_UTILS_H_
#define CLIENT_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <functional>
#include <string>
#include <netdb.h> 
#include <map>

using namespace std;

class Client_Utils{
    public: 
        Client_Utils();
        ~Client_Utils();
        void init_client(char* hostname, int port);
        string server_call(string);
        int listen_connection();
        void close_connection();
        int getPortNumber();
    private: 
        char* hostname;
        int sockfd, portno, e_code;
        struct sockaddr_in serv_addr, cli_addr;
        struct hostent *server;
        char buffer[256];
        void error(string msg);
};

#endif