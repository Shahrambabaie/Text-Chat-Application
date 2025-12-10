#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <../include/utility.hpp>
#include <vector>
#include <../include/logger.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <../include/commands.hpp>
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <typeinfo>
#include <string.h>
#include <netinet/in.h>

using namespace std;

class server { 
    static server* Object;
    
    public:
    std::vector<std::string> buf_recov_msg(string ip);
    static server* GetObject();
    list<clientInfo> clientList;
    void add_msg_to_buf(string msg, string ip);
    void server_init(char **argv, int argc);
    std::map<std::string,std::vector<std::string> > buffered_msg;
};

#endif

