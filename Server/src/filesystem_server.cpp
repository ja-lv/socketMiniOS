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
#include "../include/client_utils.h"

//headers
bool isInTable(string fileName);
string format(string msg);
string createFile(string fileName);
string deleteFile(string fileName);
string directoryListing(string msg);
string readFile(string fileName);
string writeFile(string fileName);
string dup_shell(string arg, string args);
char** s_split(string str);
string defaultRequest(string msg);
void set_max_disk(string args);
string server_read_call(int* csl);
string server_write_call(int* csl, string bytes_l_s, string data);
string makeDirectory(string dirName);
string changeDirectory(string dirName);
string printDirectory(string dirName);
string rmDirectory(string dirName);



//globals
string D_ROOT = "./storage/"; 
string D_DIR = "table";

//format 
// file map: string filename int[c, s, l] "\n"
// directory map: string directoryname, file map
map<string, map<string, int*>> directories;
string current_directory = "table";

//global
int c = 1, s = 1;
int c_max, s_max;

//client
Client_Utils client = Client_Utils();

//compile as: g++ -o ../build/basic_server basic_server.cpp server_utils.cpp
int main(int argc, char *argv[]){
    //retrieve server data
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    client.init_client(argv[1], atoi(argv[2]));
    client.listen_connection();
    cout << "Connected to client....." << endl;

    //setup cylinder max and sector max
    cout << "Setting up disk size.....: (Cylinders, Sectors) " + client.server_call("I") << endl;
    set_max_disk(client.server_call("I"));
    

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
    the_map["mkdir"] = makeDirectory;
    the_map["cd"] = changeDirectory;
    the_map["pwd"] = printDirectory;
    the_map["rmdir"] = rmDirectory;


    //infinite loop to act as a server
    while(1){
        int rspns = server.run_server_rq(the_map, defaultRequest, END_ARG);
        if(rspns > 0) break;
    }

    cout << "progam will now end.... \n" << endl;

    client.server_call("exit");

    server.close_connection();
    client.close_connection();

    return 0;
}

void set_max_disk(string args){
    int c_index = args.find(" ");
    if(c_index == string::npos){
        cout << "Error setting disk information!" << endl;
        exit(1);
    }

    string c_s = args.substr(0, c_index);
    string s_s = args.substr(c_index+1);
    int c, s;
    try{
        c = stoi(c_s);
        s = stoi(s_s);
    }
    catch(std::invalid_argument&){
        cout << "Error setting disk information!" << endl;
        exit(1);
	}

    c_max = c;
    s_max = s;
    if(c_max < 0 || s_max < 0){
        cout << "Error setting disk information!" << endl;
        exit(1);
	}
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

string makeDirectory(string dirName){
    cout << "Make directory command received" << endl;
    ofstream outfile("./storage/" + dirName + ".txt");
    return "Successfully created a directory";
}
string changeDirectory(string dirName){
    cout << "Change directory command received" << endl;
    current_directory = dirName;
    return "Current directory now changed to " + current_directory;
}
string printDirectory(string dirName){
    cout << "Print directory command received" << endl;
    return current_directory;
}
string rmDirectory(string dirName){
    cout << "Remove directory command received" << endl;
    //char filename[] = "./storage" + dirName + ".txt";
    // if(remove("./storage/"+ dirName + ".txt") != 0){
    //     return "Error deleting the directory.";
    // }
    // else{
    //     return "Successfully removed the directory";
    // }

    return "Removing directory";
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
    // int data[3] = {c, s, 0};
    directories[current_directory][fileName] = new int[3] {c, s, 0};
    // directories[current_directory][fileName][0] = c;
    // directories[current_directory][fileName][1] = s;
    // directories[current_directory][fileName][2] = 0;

    if(s > s_max && c > c_max){
        return "2. No more space!!";
    }

    if(s > s_max){
        s = 0;
        c++;
    }
    else s++;

    ofstream outfile;
    outfile.open(D_ROOT + current_directory + ".txt", std::ios_base::app);
    //ofstream outfile(D_ROOT + current_directory + ".txt");
    outfile << fileName + " " + to_string(directories[current_directory][fileName][0]) + " " + to_string(directories[current_directory][fileName][1]) + " " + to_string(directories[current_directory][fileName][2]) << endl;

    return "0. successfully created file";
}

//delete the file and remove the filename from table.txt
string deleteFile(string fileName){
    cout << "Delete file command received for " + fileName << endl;
    ifstream inFile;
    //inFile.open("./storage/table.txt");
    inFile.open("./storage/" + current_directory + ".txt");

    string line;
    string results;

    while(getline(inFile, line)){
        int pos = line.find(" ");
        string name = line.substr(0, pos);
        if(fileName == name){
            
        }
        else{
            results = results + line + "\n";
        }
    }
    //ofstream out("./storage/table.txt");
    ofstream out("./storage/" + current_directory + ".txt");
    out << results;

    return "0. Successfully deleted file";
}

//print out all the file names from table.txt
string directoryListing(string msg){
    cout << "Command received to list directory" << endl;

    if(msg == "0"){
        ifstream inFile;
        //change this to read other directories
        //inFile.open("./storage/table.txt");
        inFile.open("./storage/" + current_directory + ".txt");
        string line;
        string results;

        while(getline(inFile, line)){
            int pos = line.find(" ");
            string name = line.substr(0, pos);
            results = results + name + "\n";
        }

        return results;
    }
    if(msg == "1"){
        ifstream inFile;
        //change this to read other directories
        //inFile.open("./storage/table.txt")
        inFile.open("./storage/" + current_directory + ".txt");

        string line;
        string results;
        while(getline(inFile, line)){
            results = results + line + "\n";
        }

        return results;
    }
    else{
        return "Enter parameters either 0 or 1";
    }
}


//reads out the contents of the file
string readFile(string fileName){
    cout << "Read file command received for " + fileName << endl;
    if(!directories[current_directory][fileName]){
        return "1. No such file exists";
    }

    //make a call to the disk server
    string rspns = server_read_call(directories[current_directory][fileName]);
    
    return rspns;
}

string server_read_call(int* csl){
    int c_num = csl[0], s_num = csl[1];
    printf("Data is: %d %d\n", c_num, s_num);
    string msg = "R " + to_string(c_num) + " " + to_string(s_num);
    //send out a call
    string rspns = client.server_call(msg);
    // cout << "Sending the server the following! " + msg << endl;
    return rspns;
}

string writeFile(string msg){
    cout << "Read file command received for " + msg << endl;
    
    //parsing
    int f_index = msg.find(" ");
    if(f_index == string::npos){
        cout << "wrong arguments! W c s l data" << endl;
        return "2";
    }
    string filename = msg.substr(0, f_index);
    msg = msg.substr(f_index+1);

    //check for filename
    if(!directories[current_directory][filename]){
        return "1. No such file exists";
    }

    int l_index = msg.find(" ");
    if(l_index == string::npos){
        cout << "wrong sector size!" << endl;
        return "0";
    }
    string l_s = msg.substr(0, l_index);
    string data = msg.substr(l_index+1);

    int bytes_l;
    try{
        bytes_l = stoi(l_s);
    }
    catch(std::invalid_argument&){
        cout << "invalids W f l data! l must be an 1integer. Current W f l data: " + filename + " " + l_s + " " + data << endl;
        return "2";
    }

    //make a call to the disk server
    string rspns = server_write_call(directories[current_directory][filename], l_s, data);
    
    return rspns;
}

string server_write_call(int* csl, string bytes_l_s, string data){
    int c_num = csl[0], s_num = csl[1];
    printf("Data is: %d %d\n", c_num, s_num);
    string msg = "W " + to_string(c_num) + " " + to_string(s_num) + " " + bytes_l_s + " " + data;
    cout << "Sending the server the following! " + msg << endl;
    //send out a call
    string rspns = client.server_call(msg);
    // cout << "Sending the server the following! " + msg << endl;
    return rspns;
}