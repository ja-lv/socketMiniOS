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
#include <functional>
#include <map>
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

    int binded = 0;
    //try differnt ports until success
    while(binded == 0){
        serv_addr.sin_port = htons(portno);
        //bind the socket to our address
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
            portno++;   
        }
        else binded = 1;
    }
}

Server_Utils::~Server_Utils(){
    //close our sockets
     close(newsockfd);
     close(sockfd);
}

void Server_Utils::error(string msg){
    cout << msg + "\n";
    exit(1);
}

// takes in a function as parameter 
// returns status
// 0 = success, 1 = end argument, 2 = ERROR on accept, 3 = ERROR reading from socket, 4 = ERROR writing to socket
// default function will take in the full user message
int Server_Utils::run_server_rq(map<string, function<string (string)>> fmap, function<string (string)> default_f, string end_arg){
    if (newsockfd < 0) {
        // error("ERROR on accept");
        return 2;
    }

    bzero(buffer,256);
    int n = read(newsockfd,buffer,255);
    if (n < 0){
        // error("ERROR reading from socket");
        return 3;
    } 

    string response = (buffer);
    
    if(response.length() == 0){
        // error("ERROR no user message");
        return 0;
    }

    //sanitize to be more compatible with our server
    response = sanitize(response);

    string user_command = response.substr(0, response.find(" "));
    string user_arguments = response.substr(response.find(" ")+1);

    if(response.compare(end_arg) == 0 || user_command == end_arg){
        return 1;
    }

    string message;
    if(fmap[user_command] && user_arguments.length() > 0){
        message = fmap[user_command](user_arguments);
    }
    else{
        message = default_f(response);
    }

    //send out a message to the client
    n = write(newsockfd, message.c_str(), sizeof(char) * message.length());
    if (n < 0){
        // error("ERROR writing to socket");
        return 5;
    } 

    return 0;
}

void Server_Utils::close_connection(){
    //close our sockets
     close(newsockfd);
     close(sockfd);
}

string Server_Utils::sanitize(string str){
    if(str[str.length() - 1] == '\n'){
        str = str.substr(0, str.length() - 1);
    }
    return str;
}

int Server_Utils::getPortNumber(){
    return portno;
}

int Server_Utils::listen_connection(){
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    return 1;
}