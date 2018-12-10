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
string defaultRequest(string msg);

//globals
string D_ROOT = "./storage/"; 
string D_DIR = "table";

//format 
// file map: string filename int[c, s, l] "\n"
// directory map: string directoryname, file map
map<string, map<string, int*>> directories;
string current_directory;

//global
int c = 1, s = 1;

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
    // the_map["W"] = writeFile;


    //infinite loop to act as a server
    while(1){
        int rspns = server.run_server_rq(the_map, defaultRequest, END_ARG);
        if(rspns > 0) break;
    }

    cout << "progam will now end.... \n" << endl;

    server.close_connection();

    return 0;
}

string defaultRequest(string msg){
    cout << "message recieved: " + msg << endl;
    return "Please input valid arguments... Message recieved: " + msg;
}

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
    //IF RESET:
    //clear the whole storage folder
    //reset directories hash map

    cout << "Format File Command Received" << endl;
    //this is how we write to file
    ofstream outfile(D_ROOT + D_DIR + ".txt");
    // outfile << "created directory: " + D_DIR  << endl;
    directories[D_DIR];

    current_directory = D_DIR;

    return "Formatting file system";
}

//create a file and add the filename to table.txt
string createFile(string fileName){
    cout << "Create file command received for " + fileName << endl;

    if(directories[current_directory][fileName]){
        return "1. File already exists";
    }

    //generate hash map
    int data[3] = {c, s, 0};
    directories[current_directory][fileName] = data;

    ofstream outfile(D_ROOT + current_directory + ".txt");
    outfile << fileName + " " + to_string(directories[current_directory][fileName][0]) + " " + to_string(directories[current_directory][fileName][1]) + " " + to_string(directories[current_directory][fileName][2]) + "\n"<< endl;
    
    return "0. successfully created file";
}

//delete the file and remove the filename from table.txt
string deleteFile(string fileName){
    cout << "Delete file command received for " + fileName << endl;

    if(!isInTable(fileName)){
        return "1. File of this name does not exist";
    }

    if(remove(fileName.c_str()) != 0){
        return "2. Fail to delete file";
    }
    else{
        return "0. Successfully deleted";
    }
}

//print out all the file names from table.txt
string directoryListing(string msg){
    cout << "List Directory command received" << endl;

    ifstream infile("table.txt");
    string allDirectory((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
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
        contents = contents + line + "\n";
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