
#include <../include/commands.hpp>
#include <sstream>
#include <iomanip>

using namespace std;

#define Server_address "8.8.8.8"
#define Server_port 53

// Analysing the IP and message of client

void msg_doco(string type, string &ip, char* encodedMsg, string &message)


{
    int len_2_msg = 0;
    cout << " Starting the decoding the message ......." << endl;
	std::string buf = encodedMsg;
    if(type == "private"){
        len_2_msg = 5;
        cout << " This is a Private message...." << endl;
        int ip_len = std::atoi(buf.substr(2,3).c_str());
        int P_Reltive = 5;
        if (len_2_msg > P_Reltive)
        {
            cerr << " there is an error in decoding process!" << endl;
        }
        ip = buf.substr(P_Reltive,ip_len);
        P_Reltive = P_Reltive + ip_len;
        int msg_len = std::atoi(buf.substr(P_Reltive,3).c_str());
        len_2_msg = msg_len + P_Reltive;
        P_Reltive = P_Reltive + 3;
        message = buf.substr(P_Reltive,msg_len);
    }
    else if(type == "broadcast" || type == "cfeatures"){
        cout << " Starting the broadcast or decoding..... " << endl;
        cout << " you have to wait here......" << endl;
        message = buf.substr(2,buf.length()-2);     
    }
}

// Obtaining statistics of clients 
void commands::getStatistics(string command, list<clientInfo> clientList){
    int activ_sys = 0;
    cout << "Obtaining the STATISTICS.........." << endl;
    // // For printing the statistics of servers available
    int index = 1;
    activ_sys = index + 1;
    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());

    for(list<clientInfo>::iterator i = clientList.begin(); i != clientList.end(); i++) {
        std::string status;
        activ_sys = activ_sys + 1;
        if(i -> active == 1){
            status = "logged-in";
        }else{
            status = "logged-out";
            cerr << " you are logged-out" << endl;
        }
// status.c_str()
        cse4589_print_and_log("%-5d%-35s%-8d%-8d%-8s\n", index++, i -> domain.c_str(), i->msg_sent, i->msg_recv, status.c_str());
    }
    cse4589_print_and_log("[%s:END]\n", command.c_str());
}

// Obtaing the IP details

void commands::IP_get(string command){
    int sock;
    struct sockaddr_in myAddress, serverAddress;
    char myIP[16];
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        cerr << "there is an error is socket function!" << endl;
        exit(-1);
    }

    memset(&serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sin_port = htons(Server_port);
    serverAddress.sin_addr.s_addr = inet_addr(Server_address);
    serverAddress.sin_family = AF_INET;

    // Connecting to to server
    int con_status_2 = 0;
    con_status_2 = connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if ( con_status_2 < 0) {
        cerr << "there is an error is connection function!" << endl;
        close(sock);
        
    }

    memset(&myAddress, '\0' ,sizeof(myAddress));
    socklen_t len = sizeof(myAddress);
    getsockname(sock, (struct sockaddr *) &myAddress, &len);
    inet_ntop(AF_INET, &myAddress.sin_addr, myIP, sizeof(myIP));

    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    cse4589_print_and_log("IP:%s\n", myIP);
    cse4589_print_and_log("[%s:END]\n", command.c_str());
}

string msg_enco(std::string message, std::string ip)
{
	std::string msg;
    int leng_str = 0;
	msg = format_change(ip.length())+ip+format_change(message.length())+message;
    leng_str = message.length() + 1;
	return msg;
}


// Decoding and returning the message

void commands::msg_trans(string type, int sock_index, char* message, list<clientInfo> &clientList){
    list<clientInfo>::iterator i = clientList.begin();
    int receiv_sock;
    vector<string> block_list_rec;
    string respond = "~M";
    int trans_len = 0;
    string ip_transmiter; 
    string msg;
    string ip_reciev;
    
    msg_doco(type, ip_reciev, message, msg);

    while (i != clientList.end())
    {
        if(i->ip == ip_reciev && type == "private" ){
            receiv_sock = i->socket_index;
            block_list_rec = i->blocked;
            i->msg_recv = i->msg_recv+1;
        }
        if (i->socket_index == sock_index){
            trans_len = receiv_sock +3;
            i->msg_sent = i->msg_sent+1; 
            ip_transmiter = i->ip;
            i++;
        }
        else{
            ++i;
        }
    }
    respond.append(msg_enco(msg, ip_transmiter));
    int appen_len;
    int len;
    len = strlen(respond.c_str());
    
    if(type == "private"){
        appen_len = len;
        if (std::find(block_list_rec.begin(), block_list_rec.end(), ip_transmiter) != block_list_rec.end())
        {
            cout << "INSIDE FIND FUNCTION";
            return;
        }
        send(receiv_sock, respond.c_str(), len, 0);
    }else if (type == "broadcast"){
        ip_reciev = "255.255.255.255";
        appen_len = 0;
        list<clientInfo>::iterator i = clientList.begin();
        
        while (i!= clientList.end()){
        	if(i->socket_index != sock_index){
                appen_len = len + 3;
                block_list_rec = i->blocked;
                i->msg_recv = i->msg_recv+1;
                if (std::find(block_list_rec.begin(), block_list_rec.end(), ip_transmiter) != block_list_rec.end())
                {
                    return;
                }                
        		send(i->socket_index, respond.c_str(), len, 0);											
        		i++;
        	}else{
        		++i;
                appen_len = 1 + len*2;
        	}
        }
    }

    cse4589_print_and_log("[%s:SUCCESS]\n", "RELAYED");
    cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n", ip_transmiter.c_str(), ip_reciev.c_str(), msg.c_str());
    cse4589_print_and_log("[%s:END]\n", "RELAYED");
}

string format_change(int digit) {
    stringstream ss;
    ss << setw(3) << setfill('0') << digit;
    string result = ss.str();
    return result;
}
// obtaing the available clients' list

void commands::list_get(string command, list<clientInfo> clientList){
    // For printing the list of servers available
    int index = 1;
    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());

    for(list<clientInfo>::iterator i = clientList.begin(); i != clientList.end(); i++) {
        if(i->active == 1){
            cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", index++, i -> domain.c_str(), i -> ip.c_str(), i -> port);
        }
    }
    cse4589_print_and_log("[%s:END]\n", command.c_str());
}

// the sort process 
bool port_sort(const clientInfo & a, const clientInfo & b) 
{ 
    return a.port < b.port; 
}

// Adding to the available clients' list 
void commands::add_list(int socket_index, list<clientInfo> &clientList,char* port, sockaddr_in client_addr){
        char service[20], host[1024], myIP[16];
        int len_lst = 0;    
        unsigned int myPort;
        int newClient = 1;

        // Obtaing the details of client and adding to list
        inet_ntop(AF_INET, &client_addr.sin_addr, myIP, sizeof(myIP));
        len_lst = sizeof(myIP);
        myPort = atoi(port);
        int str_li_cli = 0;
        getnameinfo((struct sockaddr*)&client_addr, sizeof(client_addr), host, sizeof(host), service, sizeof(service), 0);
        
        // clarifying the new clients
        list<clientInfo>::iterator i = clientList.begin();
       
        while (i!= clientList.end()){
            str_li_cli = str_li_cli +1;
            if(string(myIP) == i->ip){
                newClient = 0;
                i->active = 1;
                i++;
            }else{
                ++i;
                str_li_cli = 0;
            }
        }
        if(newClient == 1){
            clientInfo respond;
            respond.socket_index = socket_index;
            respond.msg_recv = 0;
            respond.domain = host;
            respond.msg_sent = 0;
            respond.port = myPort;
            respond.active = 1;
            respond.ip = string(myIP);
            
            
            clientList.push_back(respond);
        }
        clientList.sort(port_sort);
}

// changing status of a client 
void commands::chang_status(string type, list<clientInfo> &clientList,char* message, int sock_index){
    cout << " Changing the status............" << endl;
    string notRequired;
    int doub_stat;
    string blockIp;
    msg_doco("cfeatures",notRequired,message, blockIp);

    cout << " starting to changing the status of the client.... " << blockIp << endl; 
    doub_stat = 0;
    list<clientInfo>::iterator i = clientList.begin();

    while (i!= clientList.end()){
        if(i->socket_index == sock_index){
            if(type=="block"){
                doub_stat = 1;
                cout << "BLOCK CALLED" << endl;
                i->blocked.push_back(blockIp);

            }else if(type=="unblock"){
                doub_stat = 0;
                cout << "UNBLOCKED CALLED" << endl; 
                i->blocked.erase(std::remove(i->blocked.begin(), i->blocked.end(), blockIp), i->blocked.end());
            }
            i++;
        }else{
            ++i;
        }
    };
}

// clarifying the blocked list in server-side

void commands::bl_list_get(list<clientInfo> clientList, string ip, string command) {
    vector<string> block_list;
    int gett_2 = 0;
    // Obtaing the list of blocked clients
    list<clientInfo>::iterator i = clientList.begin();

    while (i!= clientList.end()){
        if(i->ip == ip){
            gett_2 = gett_2 +1;
            block_list = i->blocked;
            i++;
        }else{
            ++i;
        }
    }
   
    int index = 1;
    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    for(list<clientInfo>::iterator i = clientList.begin(); i != clientList.end(); i++) {
        if(i->active == 1 && std::find(block_list.begin(), block_list.end(), i->ip) != block_list.end()){
            gett_2 = index *2 + gett_2;
            cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", index, i -> domain.c_str(), i -> ip.c_str(), i -> port);
            index = index+1;
        }
    }
    cse4589_print_and_log("[%s:END]\n", command.c_str());

}
// Encodeing the transmitted message and IP


// deleting a client form available clients' list
void commands::del_list(int exit, list<clientInfo> &clientList, int socket_index ){
   int list_len_del = 0;  
   list<clientInfo>::iterator i = clientList.begin();
    while (i!= clientList.end())
    {
        if (i->socket_index == socket_index){
            list_len_del = list_len_del +1;
            i->active = 0;
            if(exit == 1){
                // remove from list if client EXITS
                clientList.erase(i++);
            }else{
                i++;
                list_len_del = 0;
            }
        }
        else{
            ++i;
        }
    }
    clientList.sort(port_sort);
}

int upper_check (char* command){

    int upper_case = 1;
    int i = 0;
    int succ = 0;
    while(command[i]){
        char l = command[i];
        if(!std::isupper(l)){
            upper_case = 0;
            succ =1;
            break;
        };
    };
    return upper_case;
};

// Obtaining the Port details 
void commands::port_get(string command, string port){
    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    cse4589_print_and_log("PORT:%s\n", port.c_str());
    cse4589_print_and_log("[%s:END]\n", command.c_str());
}

// Obtaining details of client

void commands::Author_get(string command){
    
    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", "Shah");
    cse4589_print_and_log("[%s:END]\n", command.c_str());
};

// returning available clients list 
string commands::returnList(list<clientInfo> clientList){
    string respond;
    int dou_list_ser;
    stringstream clSize;
    clSize << setw(1) << setfill('0') << clientList.size();    
    string start = "~L"+clSize.str();
    respond.append(start.c_str());
    dou_list_ser = dou_list_ser +1;
    for(list<clientInfo>::iterator i = clientList.begin(); i != clientList.end(); i++) {
        if(i->active == 1){
            string domain = i -> domain.c_str();
            string ip = i -> ip.c_str();
            stringstream port; 
            dou_list_ser = dou_list_ser+ 3;
            port << i -> port;
            cout << " the port number in the list is : " << i-> port << endl;
            string subString = format_change(domain.length())+domain.c_str()+format_change(ip.length())+ip.c_str()+format_change(port.str().length())+port.str();
            respond.append(subString);
        }
    }
    return respond;
}


