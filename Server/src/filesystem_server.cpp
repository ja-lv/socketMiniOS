//constants
#include <csos_constants.hpp>

//libraries
#include <iostream>
#include <string.h>
#include <algorithm>
#include <map>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <stdio.h>
using namespace std;

//custom classes
#include "../include/server_utils.h"

//headers
string format(string msg);
string createFile(string fileName);
string deleteFile(string fileName);
string directoryListing(string msg);
string readFile(string fileName);
string writeFile(string fileName, int bytes);
string dup_shell(string arg, string args);
char** s_split(string str);

//compile as: g++ -o ../build/basic_server basic_server.cpp server_utils.cpp
int main(){
    Server_Utils server(PORT);
    cout << "Running server on port #" << to_string(server.getPortNumber()) + "\n" << endl;
    server.listen_connection();
    cout << "Booting up server.....\n" << endl;
    //setup data

    map<string, function<string (string)>> the_map;

    //setup functions
    the_map["F"] = format;
    the_map["C"] = createFile;
    the_map["D"] = deleteFile;
    the_map["L"] = directoryListing;
    the_map["R"] = readFile;
    the_map["W"] = writeFile;


    //infinite loop to act as a server
    while(1){
        int rspns = server.run_server_rq(the_map, defaultRequest, END_ARG);
        if(rspns > 0) break;
    }

    cout << "progam will now end.... \n" << endl;

    server.close_connection();

    return 0;
}



string format(string msg){
    cout << "Format File Command Received" << endl;
    ofstream outfile("table.txt");
    outfile << "" << endl;
    return "Formatting file system";
}

string createFile(string fileName){
    cout << "Create file command received for " + fileName << endl;
    ofstream outfile (fileName);

    //Add method to check file in table here
    if(false){
        return "1. File already exists"
    }
    else if(false){
        return "2. Error creating file"
    }
    else{
        //add file to table
        ofstream table;
        table.open("table.txt", ios_base::app);
        outfile << fileName <<;


        //create file
        outfile << "" << endl;
        outfile.close();
        return "0. successfully created file"
    }



}

string deleteFile(string fileName){
    cout << "Delete file command received for " + fileName << endl;
    if(remove(fileName) != 0){
        return "Failed to delete";
    }
    else{
        return fileName + " file deleted";
    }
    
}

string directoryListing(string msg){
    cout << "List Directory command received" << endl;

    ifstream infile("table.txt")
    return "{Code for list directory here}";
}

string readFile(string fileName){
    cout << "Read file command received for " + fileName << endl;
    return "{Code for reading files here}";
}

string writeFile(string fileName, int bytes){
    cout << "Write file command received for " + fileName << endl;
    return "{Code for writing files here}";
}

string dup_shell(string arg, string args){
    pid_t cid;
    int wpid, status;

    // setup pipes and child p
    int     fd[2], nbytes;
    pid_t   cid2;

    pipe(fd);

    int l_buffer = 256;
    char str[l_buffer];

    //our child will take in the pipe argument, and process it using stdout as the first argument
    if((cid = fork()) == 0){
        //close our output side, we are going to send stdout through this pipe
        close(fd[0]);

        //duplicate to pipe fd
        dup2(fd[1], STDOUT_FILENO);
        //close our write side
        close(fd[1]);

        //run the command
        execvp(arg.c_str(), s_split(args));
        
        exit(0);
    }
    else{
        //close write side of pipe
        close(fd[1]);
        //use the output of the pipe, and store it
        //read into a string
        read(fd[0], str, sizeof(str)+1);
    }

    string rspns = str;
    return rspns;
}

char** s_split(string str){
    int i;
    char* buff[10];
    char *token = strtok(strdup(str.c_str()), " ");
    while (token != NULL) {
        buff[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    return buff;
}