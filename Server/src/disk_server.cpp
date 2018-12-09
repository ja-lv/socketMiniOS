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
using namespace std;

//custom classes
#include "../include/server_utils.h"

//headers
string defaultRequest(string msg);
string disk_info(string msg);
string disk_read(string msg);
string disk_write(string msg);
string get_block(int c, int s);
string set_block(int c, int s, int l, string data);

//global variables
int disk_cylinders;
int disk_sectors;
int disk_block_buffer = 128;
//storage is a 3D array in a 2D array
//formula to access: 
// (l*M*N) + (m * N + n)
char* storage;
// = c*s*b
int storage_size;

char NULL_CHAR = '-';
string NULL_STR(disk_block_buffer, '-');

//compile as: g++ -o ../build/basic_server basic_server.cpp server_utils.cpp
int main(int argc,char* argv[]){
    int i;
    if(argc <= 2 || (stoi(argv[1]) <= 0 || stoi(argv[2]) <= 0)){
        cout << "Please enter the correct integer arguments: disk_cylinders disk_sectors.....\n" << endl;
        return 1;
    }

    //get arguments for disk cylinders and sectors
    disk_cylinders = stoi(argv[1]);
    disk_sectors = stoi(argv[2]);

    cout << "Intializing Disk storage....." << endl;

    //initialize storage
    storage_size = disk_cylinders * disk_sectors * disk_block_buffer;
    storage = (char*) malloc(storage_size);
    //set storage to nulls
    memset(storage, NULL_CHAR, storage_size);
    storage[storage_size] = (char) NULL;

    Server_Utils server(PORT);
    cout << "Running server on port #" << to_string(server.getPortNumber()) + "\n" << endl;
    server.listen_connection();
    cout << "Connection made.... Booting up disk server.....\n" << endl;
    //setup data

    map<string, function<string (string)>> the_map;

    //setup functions
    the_map["I"] = disk_info;
    the_map["R"] = disk_read;
    the_map["W"] = disk_write;

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

string disk_info(string msg){
    cout << "disk info request recieved: " + msg << endl;
    string rspns = to_string(disk_cylinders) + " " +to_string(disk_sectors);
    return rspns;
}

string disk_read(string msg){
    cout << "disk info request recieved: " + msg << endl;

    int c_index = msg.find(" ");
    if(c_index == string::npos) return "wrong arguments! R c s";
    string c_s = msg.substr(0, c_index);
    string s_s = msg.substr(c_index+1);

    int c, s;
    try{
        c = stoi(c_s);
        s = stoi(s_s);
    }
    catch(std::invalid_argument&){
		return "invalids R c s! c s must be integers.";
	}

    if((c >= 0 && c < disk_cylinders && s >= 0 && s < disk_sectors)){
        string rspns = (string) get_block(c, s);
        if(NULL_STR.compare(rspns) == 0) return "0";
        return rspns;
    }

    return "Wrong argument size..Current c s: " + to_string(c) + " " + to_string(s) 
            + ". Max c and s: " + to_string(disk_cylinders - 1) + " " + to_string(disk_sectors - 1);
}

//(l*M*N) + (m * N + n)
string get_block(int c, int s){
    //+1 for null
    char block[disk_block_buffer];
    int i;
    for(i = 0; i < disk_block_buffer; i++){
        char curr_char = storage[(c * disk_sectors * disk_block_buffer) + (s * disk_block_buffer + i)];
        block[i] = curr_char;
        if(curr_char == (char) NULL) break;
    }
    //make sure the last value is a null
    if(block[i] != (char) NULL) block[i] = (char) NULL;
    return (string) block;
}

string disk_write(string msg){
    cout << "disk info request recieved: " + msg << endl;

    int c_index = msg.find(" ");
    if(c_index == string::npos) return "wrong arguments! W c s l data";
    string c_s = msg.substr(0, c_index);
    msg = msg.substr(c_index+1);

    int s_index = msg.find(" ");
    if(s_index == string::npos) return "wrong sector size!";
    string s_s = msg.substr(0, s_index);
    msg = msg.substr(s_index+1);

    int l_index = msg.find(" ");
    if(l_index == string::npos) return "wrong l byte data size!";
    string l_s = msg.substr(0, l_index);
    string data = msg.substr(l_index+1);

    int c, s, l;
    try{
        c = stoi(c_s);
        s = stoi(s_s);
        l = stoi(l_s);
    }
    catch(std::invalid_argument&){
		return "invalids W c s l! c s l must be integers. Current c s l: " + c_s + " " + s_s + " " + l_s;
	}

    cout << "l data is: " + l_s + " and data length is: " + to_string(data.length()) << endl;

    //check for correct block buffer
    if(data.length()+1 > disk_block_buffer || l > disk_block_buffer) return "Wrong block size. Check l or data";

    if((c > 0 && c < disk_cylinders) && (s > 0 && s < disk_cylinders)){
        string rspns = set_block(c, s, l, data);
        return "added " + rspns + " and on paper: " + get_block(c, s);
    }

     return "Wrong argument size";
}

string set_block(int c, int s, int l, string data){
    //+1 for null
    char block[disk_block_buffer];
    int i;
    for(i = 0; i < l+1; i++){
        char curr_char = data.c_str()[i];
        int curr_i = (c * disk_sectors * disk_block_buffer) + (s * disk_block_buffer + i);
        block[i] = curr_char;
        //if last add and break
        if(i == l) storage[curr_i] = (char) NULL;
        else storage[curr_i] = curr_char;
    }
    //make sure the last value is a null
    if(block[i] != (char) NULL) block[i] = (char) NULL;
    return (string) block;
}