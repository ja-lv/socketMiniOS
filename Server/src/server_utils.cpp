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
    //set our hostname
    //print out our hostname
    char ghn[1024];
    gethostname(ghn, 1024);
    this->hostname = string(ghn);

    cout << "Hostname is: " << this->hostname + "\n";

    //initialize socket information
    cout << to_string(port) + "yraaa \n";
};