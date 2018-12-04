#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#include <string>

class Server_Utils{
    public: 
        Server_Utils(int port);
    private: 
        std::string hostname;
};

#endif