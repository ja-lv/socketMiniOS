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
bool isInTable(string fileName);
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


//idea is to use table.txt as a manager for all the files in the directory. table.txt
//will store all the filenames and when you create a file table.txt will update and
//record the filename. When you delete a file table.txt will remove that filename


bool isInTable(string fileName){
    ifstream file("table.txt");
    string line;

    while(getline(file, line)){
        if(line == fileName){
            return true;
        }
    }

    return false;
}

//intialize the table
string format(string msg){
    cout << "Format File Command Received" << endl;
    ofstream outfile("table.txt");
    outfile << "" << endl;
    return "Formatting file system";
}

//create a file and add the filename to table.txt
string createFile(string fileName){
    cout << "Create file command received for " + fileName << endl;
    ofstream outfile (fileName);

    if(isInTable(fileName)){
        return "1. File already exists";
    }
    }
    else{
        //add file to table
        ofstream table;
        table.open("table.txt", ios_base::app);
        outfile << fileName;


        //create file
        outfile << "" << endl;
        outfile.close();
        return "0. successfully created file";
    }



}

//delete the file and remove the filename from table.txt
string deleteFile(string fileName){
    cout << "Delete file command received for " + fileName << endl;

    if(!isInTable(fileName)){
        return "1. File of this name does not exist";
    }

    if(remove(fileName) != 0){
        return "2. Fail to delete file";
    }
    else{
        return "0. Successfully deleted";
    }
    
}

//print out all the file names from table.txt
string directoryListing(string msg){
    cout << "List Directory command received" << endl;

    ifstream infile("table.txt")
    string allDirectory = ((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    return allDirectory;
}


//reads out the contents of the file
string readFile(string fileName){
    cout << "Read file command received for " + fileName << endl;
    if(!isInTable(fileName)){
        return "1. No such filename exists";
    }

    string line;
    string contents;
    ifstream input (fileName);
    while(getline (input, line)){
        content = content + line + "\n";
    }
    return contents;
}

string writeFile(string fileName, int bytes){
    cout << "Write file command received for " + fileName << endl;
    if(!isInTable(fileName)){
        return "1. No such filename exists";
    }

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