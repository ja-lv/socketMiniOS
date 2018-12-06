#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <functional>
#include <string>
#include <map>

using namespace std;

class Server_Utils{
    public: 
        Server_Utils(int port);
        ~Server_Utils();
        int run_server_rq(map<string, function<string (string)>> fmap, function<string (string)> default_f, string end_arg);
        int listen_connection();
        void close_connection();
        int getPortNumber();
    private: 
        string hostname;
        int sockfd, newsockfd, portno;
        socklen_t clilen;
        struct sockaddr_in serv_addr, cli_addr;
        char buffer[256];
        void error(string msg);
        string sanitize(string str);
};

#endif