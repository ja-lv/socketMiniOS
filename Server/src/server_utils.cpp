//headers
#include "../include/server_utils.h"
//dependencies
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//c++ libraries
#include <iostream>
using namespace std;

Server_Utils::Server_Utils(int port){
    //check for port
    if(port <= 0){
        error("Error, no port!!\n");
    }

    //set our hostname
    //print out our hostname
    char ghn[1024];
    gethostname(ghn, 1024);
    hostname = string(ghn);

    cout << "Hostname is: " << hostname + "\n";

    //initialize socket information
    //create our socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = port;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //bind the socket to our address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
};

void Server_Utils::error(string msg){
    cout << msg + "\n";
    exit(1);
}

void Server_Utils::run_server(){
    cout << "Booting up server.....\n";

    //infinite loop to act as a server
     while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");

        bzero(buffer,256);
        int n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n",buffer);

        //send out a message to the client
        n = write(newsockfd,"I got your message",18);
        if (n < 0) error("ERROR writing to socket");
    }
}