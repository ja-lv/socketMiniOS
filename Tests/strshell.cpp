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

string dup_shell(string arg, string args);

 //compile as: g++ -o strshell strshell.cpp
int main(){

    cout << dup_shell("ls", "\0");

    return 0;
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
        if(args.length() > 0){
            execlp(arg.c_str(), arg.c_str(), args.c_str(), NULL);
        }
        else{
            execlp(arg.c_str(), arg.c_str(), NULL);
        }
        
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