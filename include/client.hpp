#ifndef CLIENT_HPP_
#define CLIENT_HPP_



#include <../include/commands.hpp>
#include <../include/logger.h>
#include <../include/utility.hpp>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>
#include <strings.h>
#include <stdio.h>
#include <vector>
#include <netdb.h>
#include <stdlib.h>


using namespace std;



class client
{
    int active_cl, status_sock;
    int sock_first;
    int server_status;
    int serverSocket;
    static client* Object;
    std::string listeningPort;
    fd_set master_list,copy2_list, copy_list;


    public:
    std::vector<std::string> block_list;
    list<clientInfo> clientList;
    int Ch_block_list(std::string ip);
    void stat_chang_to_unblock(std::string ip);
    void client_init(char **argv, int argc);
    void command_handle();
    int con_to_serv(const char* server_port, const char *server_ip);
    void serv_msg_hangle(char *buffer);
    int Check_client(std::string clientIP);    
    static client* GetObject();

};

#endif

