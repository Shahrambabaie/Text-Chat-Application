#ifndef COMMANDS_HPP_
#define COMMANDS_HPP_

#include <../include/commands.hpp>
#include <../include/logger.h>
#include <bits/stdc++.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <strings.h>
#include <stdio.h>

using namespace std;

struct clientInfo
{
    
    vector<string> blocked;
    int msg_sent;
    int port;
    int active;
    int msg_recv;
    int socket_index;
    string domain;
    string ip;
};
string format_change(int digit);
class commands {
    
    public:
    string returnList(list<clientInfo> clientList);
    void IP_get(string command);
    void getStatistics(string command, list<clientInfo> clientList);
    void del_list(int exit, list<clientInfo> &clientList, int socket_index);
    void port_get(string command, string port);
    void bl_list_get(list<clientInfo> clientList, string ip, string command);
    void Author_get(string command);
    void msg_trans(string type, int sock_index, char* message, list<clientInfo> &clientList);
    void list_get(string command, list<clientInfo> clientList);
    void chang_status(string type, list<clientInfo> &clientList, char* message, int sock_index);
    void add_list(int socket_index, list<clientInfo> &clientList, char* port, sockaddr_in client_addr);


};

#endif

