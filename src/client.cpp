

#include <../include/client.hpp>

#define FALSE 0
#define TRUE 1
#define MsdSize 256
#define flag 0
#define BufSize  1024

using namespace std;
int i;
int blocked_num = 0;

client* client::Object = NULL;
client* client::GetObject() 
{
   if (Object != NULL)
   {
       return(Object);
   }
   else
   {
      Object = new client();
      return(Object);
   }
}


void str_list_deco(char *buffer, list<clientInfo> &clientList)
{   
    int P_Reltive = 3;
    int Clinet_count = 0;
    clientList.clear();
    std::string buf = buffer;
    Clinet_count = std::atoi(buf.substr(2,1).c_str());
    for(i = 0; i < Clinet_count; ++i)
    {
        int Length_d = std::atoi(buf.substr(P_Reltive,3).c_str());
        cout<<"Domain len="<<Length_d<<"\n";
        P_Reltive = P_Reltive + 3;
        std::string domain = buf.substr(P_Reltive,Length_d);
        P_Reltive = P_Reltive + Length_d;
        cout<<"The Domain is equal to:"<<domain<<"\n";

        int Len1_IP;
        Len1_IP = std::atoi(buf.substr(P_Reltive,3).c_str());
        cout<<"The IP length is:"<<Len1_IP<<"\n";
        P_Reltive = P_Reltive + 3;
        std::string ip = buf.substr(P_Reltive,Len1_IP);
        P_Reltive = P_Reltive + Len1_IP;
        cout<<"The IP equals to:"<< ip <<"\n";
        
        int port_len = 0;
        port_len = std::atoi(buf.substr(P_Reltive,3).c_str());
        cout<<"The Length of port is equal to:"<<port_len<<"\n";
        P_Reltive = P_Reltive + 3;
        int port = std::atoi(buf.substr(P_Reltive,port_len).c_str());
        P_Reltive = P_Reltive+ port_len;
        cout<<"The connected PORT is equal to:"<< port <<"\n";
        //cse4589_print_and_log("The connected PORT is equal to %s", port);

        
        
         
        clientInfo respond;
        respond.domain = string(domain);
        respond.ip = string(ip);
        respond.port = port;

        clientList.push_back(respond);


    }
    P_Reltive = 0;
    
}


void client :: client_init(char **argv, int count_arg)
{
    if(count_arg != 3) {
        int status_new = 0;
        cse4589_print_and_log ("The usage is:%s [ip] [port]\n", argv[0]);
        exit(1);
    }
    std::vector<std::string> comm_inst;
    client* obj_client = client::GetObject();
    char BUFFER [BufSize] ={0};

    memset(&obj_client->server_status,0,sizeof(int));
    memset(BUFFER, 0, BufSize);
    memset(&obj_client->serverSocket,0,sizeof(int));


  
    comm_inst.assign(argv + 1,  count_arg + argv);
    FD_ZERO(&obj_client->copy_list);
    obj_client->listeningPort = comm_inst[1];
    
    // Clearing the sets
    FD_ZERO(&obj_client->copy2_list);
    FD_ZERO(&obj_client->master_list);
    
    //Adding a file descriptor to the set
    FD_SET(flag, &obj_client->master_list);
    
    active_cl = 0;
    obj_client->sock_first = 0;
    cout << "\n[PA1-CLIENT@CSE489/589]$ " << endl;
    fflush(stdout);
    int status,sock_index;

    while(TRUE)
    {   
        memcpy(&obj_client->copy_list, &obj_client->master_list, sizeof(obj_client->master_list));
        fd_set copy2_list = master_list;
        status = select(obj_client->sock_first + 1, &obj_client->copy_list, NULL, NULL, NULL);
        if(status < 0)
            cerr << "There is a problem is select function!" << endl;
        // checking that if there is a new connection to process
        if(status > 0)
        {   
            fd_set copy2_list = master_list;
            for(sock_index=0; sock_index<=obj_client->sock_first; sock_index+=1)
            {   
                if(FD_ISSET(sock_index, &obj_client->copy_list))
                {
                    // Checking if there is a new command
                    if (sock_index == flag)
                    {
                        obj_client->command_handle();
                        active_cl = sock_index +1;
                    }
                    else if(sock_index == obj_client->serverSocket)
                    {
                        fd_set copy2_list = master_list;
                        char *buffer = (char*) malloc(sizeof(char)*BufSize );

                        //Handle once server is configured
                        cout << "Analyzing the socket of server...." << endl;
                        memset(buffer, '\0', BufSize );
                        
                        if(recv(sock_index, buffer, BufSize , 0) <= 0){
                            close(sock_index);
                            cout << "The remote client closed the connection!\n" << endl;
                            
                        }
                        else {
                            
                            cout << " The server transmitted that: %s\n" << buffer << endl;
                            cout << "Replying it to the remote client ... \n"<< endl;
                            obj_client->serv_msg_hangle(buffer);

                            
                        }
                        
                        free(buffer);                       
                    }
                    else
                    {
                        cout << " This is the invalid Socket!"<< endl;
                    }
                }
            }
        }
        cout << "\n[PA1-CLIENT@CSE489/589]$ " << endl;
        fflush(stdout);
    }
}

int client :: con_to_serv(const char* server_port, const char *server_ip)
{
	int sock_status;
    active_cl = 0;
	int true_2 = 1;
	struct addrinfo hints, *res;

	// the hint structure
	memset(&hints, 0, sizeof(hints));
	
    int sock_status_2 = 0;

	// obtaing the address structures	
	if (getaddrinfo(server_ip, server_port, &hints, &res) != 0)
	{
		cerr << " There is a problem in obtaing information!" << endl;
		return 0;
	}
	// socket structure
	sock_status = socket(AF_INET, SOCK_STREAM, res->ai_protocol);
	if(sock_status < 0)
	{
		cerr << "There is a problem is creating the socket!" << endl;
		return 0;
	}

    sock_status_2 = setsockopt(sock_status, SOL_SOCKET, SO_REUSEADDR, &true_2, sizeof(int));
	if ( sock_status_2 == -1) {
		cerr << "there is an error is set socket function!" << endl;
		exit(-1); 
	}
    sock_status_2 = setsockopt(sock_status, SOL_SOCKET, SO_REUSEPORT, &true_2, sizeof(int));
	if ( sock_status_2 == -1) {
		cerr << "there is an error is set socket function!" << endl;
		exit(-1); 
	}	
	// the Connect structure
    int status_conn = 0;
    status_conn = connect(sock_status, res->ai_addr, res->ai_addrlen);
	if( status_conn < 0)
	{
		cerr << "there is an error is set connection function!" << endl;
		return 0;
	}
	freeaddrinfo(res);

	client::GetObject()->serverSocket = sock_status;
	return TRUE;

}
int client :: Check_client(std::string clientIP)
{
    client* obj_client = client::GetObject();
    list<clientInfo>::iterator i = obj_client->clientList.begin();
    for( list<clientInfo>::iterator i = obj_client->clientList.begin(); i != obj_client->clientList.end(); i++) 
    {
        if(clientIP == i->ip)
            return 1;
    }
    return 0;
}
int IP_ckeck(std::string ip)
{   
    struct sockaddr_in sa;
    
    int res = inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr));
    if(res == TRUE)
        return 1;
    else
        return 0;
}

int Check_port(std::string port)
{
    for(i = 0; i < port.length(); ++i)
    {
        if(!isdigit(port[i]))
        {
            return 0;
        }
    }
    return TRUE;
}



std::string encodeMsg(std::string ip,std::string message)
{
    int endo_len = 0;
    std::string msg;
    msg = "~M"+format_change(ip.length())+ip+format_change(message.length())+message;
    endo_len = message.length() +3;
    return msg;
}

int client :: Ch_block_list(std::string ip)
{
    client* obj_client = client::GetObject();
    for(i = 0; i < obj_client->block_list.size();++i)
    {
        if(obj_client->block_list[i] == ip)
            return 1;
    }
    return 0;
}
void client :: stat_chang_to_unblock(std::string ip)
{   
    int new_stat;
    client* obj_client = client::GetObject();
    obj_client->block_list.erase(std::remove(obj_client->block_list.begin(), obj_client->block_list.end(), ip), obj_client->block_list.end());
     new_stat = 0;
}


void client :: command_handle()
{   

    int arg_count = 0;
    commands cmd_inst;
    std::string msg_circl;
    std::string msg_buf_send;
    std::string port_num_send;
    std::string broadCast;
    client* obj_client = client::GetObject();
    std::string command;
    std::getline(std::cin,command);
    std::istringstream iss(command);
    std::vector<std::string> com_vector;
    std::string token;
    
    com_vector.clear();


    while(std::getline(iss, token, ' '))
    {
        com_vector.push_back(token);
        arg_count = 1 + arg_count;
    }
    msgType msg = Type_msg_get(com_vector[0]);
    switch(msg)
    {

        case LIST:
            if(!obj_client->server_status)
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            cmd_inst.list_get(com_vector[0], obj_client->clientList);
            break;

        case PORT:
            cmd_inst.port_get(com_vector[0], obj_client->listeningPort );
            break;
        case BROADCAST:
            if(!obj_client->server_status)
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            for(i = 1; i < com_vector.size(); ++i)
            {
                msg_circl = msg_circl + com_vector[i]+ " ";
            }
            active_cl = status_sock + com_vector.size();
            msg_circl[msg_circl.length()-1] == '\0';
            cout<<"msg_circl:"<<msg_circl<< endl;
            cout << " new message....." << endl;
            cout << "You can try another message!" << endl;
            msg_buf_send = "~B";
            status_sock = 1;
            msg_buf_send.append(msg_circl);
            cout<<"Sending broadcast message"<<msg_buf_send<<endl;
            active_cl = status_sock + com_vector.size();
            send(obj_client->serverSocket,msg_buf_send.c_str(),msg_buf_send.length(),0);
            cse4589_print_and_log("[%s:SUCCESS]\n", com_vector[0].c_str());
            cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
            cerr << "Problem, Try again\n" << com_vector[0].c_str()<< endl;
            break;
        case IP:
            cmd_inst.IP_get(com_vector[0]);
            break;    
            
        case AUTHOR:    
            cmd_inst.Author_get(com_vector[0]);
            break;

        case LOGIN:
            if(!IP_ckeck(com_vector[1]))
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            if(!Check_port(com_vector[2]))
            {   
                cout<<"it's a valid ip\n";
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            if(!obj_client->con_to_serv(com_vector[2].c_str(), com_vector[1].c_str()))
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            if(obj_client->server_status)
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            obj_client->server_status = TRUE;
            status_sock = 0;
            FD_SET(obj_client->serverSocket, &obj_client->master_list);
            obj_client->sock_first = obj_client->serverSocket + 1;
            port_num_send = "~P" + obj_client->listeningPort;
            //port_num_send = "~P" + obj_client->listeningPort;
            send(obj_client->serverSocket, port_num_send.c_str(), port_num_send.length(), 0);   
            status_sock = 1 + status_sock;
            cse4589_print_and_log("[%s:SUCCESS]\n", com_vector[0].c_str());
            cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
            cout<<"Connected to server\n";
            break;
    
        
        
        
        

        case SEND:
            if(!obj_client->server_status)
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            if(!IP_ckeck(com_vector[1]))
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            if(!(obj_client->Check_client(com_vector[1])))
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }

            cout << "\nSENDing it to the remote server ... \n" << endl;
            for(i = 2; i < com_vector.size(); ++i)
            {
                active_cl = 0; 
                msg_circl = msg_circl + com_vector[i]+ " ";
            }
            msg_circl[msg_circl.length()-1] == '\0';
            cout<<"msg_circl:"<<msg_circl<<endl;
            msg_buf_send = encodeMsg(com_vector[1],msg_circl);
            cout<<"Encoded msg : --> "<<msg_buf_send<<endl;
            if(send(obj_client->serverSocket, msg_buf_send.c_str(), msg_buf_send.length(), 0) > 0)
            {
                cout << "Done!\n" << endl;
                cse4589_print_and_log("[%s:SUCCESS]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
            }
            else
            {   
                cout<<"Msg not sent to server\n";
                active_cl = 3;
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
            }
            break;
        case BLOCK:
			if(!obj_client->server_status)
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
			if(!IP_ckeck(com_vector[1]))
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            if(!(obj_client->Check_client(com_vector[1])))
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            if(obj_client->Ch_block_list(com_vector[1]))
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
			msg_buf_send = "~K" + com_vector[1];
			cout<<"Sending block message"<<msg_buf_send<<endl;
            send(obj_client->serverSocket,msg_buf_send.c_str(),msg_buf_send.length(),0);
            obj_client->block_list.push_back(com_vector[1]);
            cse4589_print_and_log("[%s:SUCCESS]\n", com_vector[0].c_str());
            cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
			break;
    

            
        case REFRESH:
            if(!obj_client->server_status)
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            send(obj_client->serverSocket, "~R", 2, 0); 
            cse4589_print_and_log("[%s:SUCCESS]\n", com_vector[0].c_str());
            cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
            break;
		
		
		case UNBLOCK:
			if(!obj_client->server_status)
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
			if(!IP_ckeck(com_vector[1]))
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            if(!(obj_client->Check_client(com_vector[1])))
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            if(!obj_client->Ch_block_list(com_vector[1]))
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }

            obj_client->stat_chang_to_unblock(com_vector[1]);
			msg_buf_send = "~U" + com_vector[1];
			cout<<"Sending unblock message"<<msg_buf_send<<endl;
            send(obj_client->serverSocket,msg_buf_send.c_str(),msg_buf_send.length(),0);
            cse4589_print_and_log("[%s:SUCCESS]\n", com_vector[0].c_str());
            cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
			break;


        case LOGOUT:
            if(!obj_client->server_status)
            {
                cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
                cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
                break;
            }
            close(obj_client->serverSocket);
            cout << "Remote Host terminated connection!\n"<< endl;
            // Remove client from list of clients

            obj_client->clientList.clear();
            FD_CLR(obj_client->serverSocket, &obj_client->master_list);
            obj_client->sock_first = 0;
            obj_client->server_status = FALSE;
            cse4589_print_and_log("[%s:SUCCESS]\n", com_vector[0].c_str());
            cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
            break;
        
        case EXIT:
            if(obj_client->server_status)
            {   
                send(obj_client->serverSocket, "~E", 2, 0); 
                close(obj_client->serverSocket);
                FD_CLR(obj_client->serverSocket, &obj_client->master_list);
            }
            cse4589_print_and_log("[%s:SUCCESS]\n",com_vector[0].c_str());
            cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
            exit(1);
            break;
        case NOTFOUND:
            cse4589_print_and_log("[%s:ERROR]\n", com_vector[0].c_str());
            cse4589_print_and_log("[%s:END]\n", com_vector[0].c_str());
            break;


    }
}


void receiv_msg_handl(char *buffer)
{   
    int Len2_IP = 0;
    std::string buf = buffer;
    Len2_IP = std::atoi(buf.substr(2,3).c_str());
    int P_Reltive = 5;
   

    std::string ip = buf.substr(P_Reltive,Len2_IP);
    P_Reltive = P_Reltive + Len2_IP;
    int msg_len = std::atoi(buf.substr(P_Reltive,3).c_str());
    P_Reltive = P_Reltive + 3;
    std::string msg = buf.substr(P_Reltive,msg_len);
    cse4589_print_and_log("[%s:SUCCESS]\n","RECEIVED");
    cse4589_print_and_log("msg from:%s\n[msg]:%s\n", ip.c_str(), msg.c_str());
    cse4589_print_and_log("[%s:END]\n", "RECEIVED");
}

void client :: serv_msg_hangle(char *buffer)
{   
    int new_statos = 0;
    client* obj_client = client::GetObject();
    if(buffer[1] == 'L' && buffer[0] == '~' )
    {
        active_cl = status_sock - active_cl;
        cout<<"It is a list msg\n";
        new_statos = 1;
        str_list_deco(buffer, obj_client->clientList);
    }
    else if(buffer[1] == 'M' && buffer[0] == '~')
    {
        cout<<"It is regular msg from client\n";
        receiv_msg_handl(buffer);
    }
}
