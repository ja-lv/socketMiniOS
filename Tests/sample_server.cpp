/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
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

void error(const char *msg){
    perror(msg);
    exit(1);
}

//compiled as g++ sample_server.cpp -o ../build/sample_server
//run: ../build/sample_server
int main(int argc, char *argv[]){
    //print out our hostname
    char hostname[1024];
    gethostname(hostname, 1024);
    string name = string(hostname);
    cout << "Hostname is: " << name + "\n";

    //set our socket, and the socketfd to respond to
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     //create our socket
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     //bind the socket to our address
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     //recieve the message and accept, print our the buffer message
     //keep track of the incoming socket so we can respond

     //infinite loop to act as a server
     while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");

        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n",buffer);

        //send out a message to the client
        n = write(newsockfd,"I got your message",18);
        if (n < 0) error("ERROR writing to socket");
    }

     //close our sockets
     close(newsockfd);
     close(sockfd);
     return 0; 
}