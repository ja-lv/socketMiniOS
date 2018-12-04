//dependencies
#include <string.h>
using namespace std;
//custom classes
#include "../include/server_utils.h"

int PORT = 2035;

//compile as: g++ -o ../build/basic_server basic_server.cpp server_utils.cpp
int main(){
    Server_Utils server(PORT);
    server.run_server();
    return 0;
}