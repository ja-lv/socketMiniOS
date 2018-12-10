//headers
#include "../include/client_utils.h"

//dependencies
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

//c++ libraries
#include <iostream>
#include <functional>
#include <map>
using namespace std;

Client_Utils::Client_Utils(){
}

Client_Utils::~Client_Utils(){
    //close our sockets
    close_connection();
}

void Client_Utils::init_client(char* hn, int port){
    //check for port
    if(port <= 0){
        error("Error, no port!!\n");
    }

    //set our hostname and port
    portno = port;
    hostname = hn;

    //setup our socket based on port
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
}

void Client_Utils::error(string msg){
    cout << msg + "\n";
    exit(1);
}

void Client_Utils::close_connection(){
    //close our sockets
     close(sockfd);
}

int Client_Utils::getPortNumber(){
    return portno;
}

int Client_Utils::listen_connection(){
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR connecting");
    }
    return 1;
}

string Client_Utils::server_call(string msg){
    int n = write(sockfd,msg.c_str(),msg.length());
    if (n < 0){
        error("ERROR writing to socket");
    }
    bzero(buffer,256);
    //read incoming info from the server
    n = read(sockfd,buffer,255);
    if (n < 0) {
        error("ERROR reading from socket");
    }

    return (string) buffer;
}