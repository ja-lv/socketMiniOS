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
string test(string msg);
string defaultRequest(string msg);
string str_reverse(string msg);
string ls(string msg);
string dup_shell(string arg, string args);
char** s_split(string str);

//compile as: g++ -o ../build/basic_server basic_server.cpp server_utils.cpp
int main(){
    Server_Utils server(PORT);
    cout << "Running server on port #" << to_string(server.getPortNumber()) + "\n" << endl;
    server.listen_connection();
    cout << "Connection made.... Booting up disk server.....\n" << endl;
    //setup data

    map<string, function<string (string)>> the_map;

    //setup functions
    the_map["reverse"] = str_reverse;

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

string str_reverse(string msg){
    cout << "message recieved, to be reversed: " + msg << endl;
    if(msg == "\0"){
        return defaultRequest("\0");
    }
    string copy = msg;
    reverse(copy.begin(), copy.end());
    return copy;
}