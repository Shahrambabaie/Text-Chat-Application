
#include <../include/server.hpp>

#define BACKLOG 5
#define STDIN 0
#define TRUE 1
#define CMD_SIZE 100
#define BufSize  1024

server* server::Object = NULL;
commands cmd_inst;
server* server::GetObject() 
{
  if (Object != NULL)
   {
       return(Object);
   }
   else
   {
      Object = new server();
      return(Object);
   }
}

int sock_st_2 = 0;
void server :: server_init(char **argv, int argc)
{	
	if(argc != 3) {
		cout << "Usage:%s [port]\n" << argv[1]<< endl;
		exit(-1);
	}
	
	server* object_serverobj = server::GetObject();
	int true_2 = 1, select_stat, server_socket, head_socket, sock_index ;
	int accept_status=0;
    socklen_t caddr_len;
	struct sockaddr_in client_addr;
	struct addrinfo hints, *res;
	fd_set master_list, copy4_list, copy3_list;

	// the hint structure
	memset(&hints, 0, sizeof(hints));
    	hints.ai_flags = AI_PASSIVE;

	// obtaing the address structures
	if (getaddrinfo(NULL, argv[2], &hints, &res) != 0)
		cerr << "there is an error is obtaing the information!" << endl;
	
	// the Socket structure
	server_socket = socket(AF_INET, SOCK_STREAM, res->ai_protocol);
	if(server_socket < 0)
		cerr << "there is an error is creating the socket!" << endl;

	
    sock_st_2 = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &true_2, sizeof(int));
	if ( sock_st_2 == -1) {
		cerr << "there is an error is set socket function!" << endl;
		exit(1); 
	}
	sock_st_2 = setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &true_2, sizeof(int));
	if ( sock_st_2 == -1) {
		cerr << "there is an error is set socket function!" << endl;
		exit(1); 
	}
	// the Bind structure
	int bind_status = 0;
	bind_status = bind(server_socket, res->ai_addr, res->ai_addrlen);
	if( bind_status < 0)
		cerr << "there is an error is bing function!" << endl;


	
	freeaddrinfo(res);
	
	cout << "\n[PA1-SERVER@CSE489/589]$ " << endl;
	fflush(stdout);	

	// the Listen structure
	int lis_status = 0;
	lis_status = listen(server_socket, BACKLOG);

	if(lis_status < 0)
		cerr << "there is an error is listening function!" << endl;

	// clearing the sets
	FD_ZERO(&copy3_list);
	FD_ZERO(&copy4_list);
	FD_ZERO(&master_list);
	
	
	// Registering the listening socket
	FD_SET(server_socket, &master_list);
	FD_SET(STDIN, &master_list);
	
	head_socket = server_socket;
	while(TRUE){
		memcpy(&copy3_list, &master_list, sizeof(master_list));
		// the select function
		select_stat = select(head_socket + 1, &copy3_list, NULL, NULL, NULL);
		if(select_stat < 0)
		cerr << "there is an error is select function!" << endl;

		// analysing that if there is a new to process
		if(select_stat > 0){
			// checking socket descriptors that if there is a ready 
			for(sock_index=0; sock_index<=head_socket; sock_index+=1){
				
				if(FD_ISSET(sock_index, &copy3_list)){
					// checking that if there is a new command
					if (sock_index == STDIN){
						char *cmd = (char*) malloc(sizeof(char)*CMD_SIZE);
						
						memset(cmd, '\0', CMD_SIZE);
						if(fgets(cmd, CMD_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to cmd
							exit(-1);
						
						cout << "\n there is a new one: %s\n" << cmd << endl;

						// Processing the PA1 commands
						char *arg = strtok(cmd," ");
						std::string token;
						std::vector<std::string> cmdArgv;
						while(arg != NULL){
							token = arg;
							int len = token.length();
							if(token[len-1] == '\n')
							{
								token.erase(len-1);
							}
							cmdArgv.push_back(token);
							arg = strtok(NULL," ");
						}
						msgType msg;
						memset(&msg, 0, sizeof(msgType));
						msg = Type_msg_get(cmdArgv[0]);
						switch(msg) 
						{
							case STATISTICS:
								cmd_inst.getStatistics(cmdArgv[0], object_serverobj->clientList);
								break;
							case LIST:
								cmd_inst.list_get(cmdArgv[0], object_serverobj->clientList);
								break;
							case NOTFOUND:
								cse4589_print_and_log("[%s:ERROR]\n", cmdArgv[0].c_str());
								cse4589_print_and_log("[%s:END]\n", cmdArgv[0].c_str());
								break;	
							case IP:
								cmd_inst.IP_get(cmdArgv[0]);
								break;	
							case BLOCKED:
								cmd_inst.bl_list_get(object_serverobj->clientList, cmdArgv[1], cmdArgv[0] );
								break;
							case PORT:
								cmd_inst.port_get(cmdArgv[0], argv[2]);
								break;		
							case AUTHOR:
								cmd_inst.Author_get(cmdArgv[0]);
								break;
						
																							
							
						}
						free(cmd);
					}
					// Checking that if there is a new connection request
					else if(sock_index == server_socket){
						caddr_len = sizeof(client_addr);
						accept_status = accept(server_socket, (struct sockaddr *)&client_addr, &caddr_len);
						if(accept_status < 0)
							cerr << "there is an error is accept function!" << endl;

						
						cout << "\nRemote Host connected!" << endl;                      
						            
						FD_SET(accept_status, &master_list);
						if(accept_status > head_socket) head_socket = accept_status;						
					}
					// Reading from the connected clients
					else{
						// initializing the buffer to get the response
						char *buffer = (char*) malloc(sizeof(char)*BufSize );
						memset(buffer, '\0', BufSize );
						
						if(recv(sock_index, buffer, BufSize , 0) <= 0){
							// Remove client from list of clients if logged out
							cmd_inst.del_list(0, object_serverobj->clientList, sock_index);
							close(sock_index);
							cout << "Remote Host terminated connection!\n" << endl;

							// Removing from the list
							FD_CLR(sock_index, &master_list);
						}
						else {
							//Process incoming data from existing clients here ...
							cout << "\n the host transmitted to me: %s\n" << buffer << endl;
							cout << "Replying this to the remote client ... \n" << endl;

							msgType msg;
							string str_buf = buffer;
							string pCmd = str_buf.substr(0,2);
							msg = Type_msg_get(pCmd);
							string respond;
							switch(msg) {
                                case BROADCAST:
									{
										cout << "This is the broadacst command... " <<endl;
										cmd_inst.msg_trans("broadcast", sock_index,buffer, object_serverobj->clientList);
										break;											
									}

                                case REFRESH:
									{
										cout << " Statring the refreshing command.... " << endl;
										// return list of avilable clients on login
										respond = cmd_inst.returnList(object_serverobj->clientList);
										cout << " Returing the string..... " << respond << endl;
										int len;
										len = strlen(respond.c_str());
										send(sock_index, respond.c_str(), len, 0);										
										break;
									}

                                case UNBLOCK:
									{
										cout << " this is unblock command.... " <<endl;
										cmd_inst.chang_status("unblock", object_serverobj->clientList,buffer, sock_index);
										break;											
									}

                                case EXIT:
									{
										cout << " This is the exit command..... " << endl;
										// Remove client from list of clients if logged out
										cmd_inst.del_list(1, object_serverobj->clientList, sock_index);
									}

									
								
								case ADDCLIENT:
									{
										cout << " This is the add client command...." << endl;
										// Add new client to list of clients
										char port[5];
										strncpy(port,buffer+2,5);
										cmd_inst.add_list(accept_status, object_serverobj->clientList,port, client_addr);
										// return list of avilable clients on login
										cout << " The list returning to the host....." << endl;
										respond = cmd_inst.returnList(object_serverobj->clientList);
										int len;
										len = strlen(respond.c_str());
										send(accept_status, respond.c_str(), len, 0);
										break;
									}
								
								case SEND:
									{
										cmd_inst.msg_trans("private", sock_index,buffer, object_serverobj->clientList);	


										break;
									}
								
								case BLOCK:
									{
										cout << "This is the block command... " <<endl;
										cmd_inst.chang_status("block", object_serverobj->clientList,buffer, sock_index);
										break;											
									}
								
																										
							}

							// if(send(accept_status, buffer, strlen(buffer), 0) == strlen(buffer))
							cout << " it is OK! \n" << endl;
							fflush(stdout);
						}
						
						free(buffer);
					}
				}
			}
		}
	cout << "\n[PA1-SERVER@CSE489/589]$ "<< endl;
	fflush(stdout);
	}
}


void server :: add_msg_to_buf(string msg, string ip)
{
	server* object_serverobj = server::GetObject();
	int stack_st = 0;
	std::map<std::string,std::vector<std::string> >::iterator it = object_serverobj->buffered_msg.find(ip);
	if (sizeof(ip) < stack_st)
	     cerr << "There is a problem in caling the function!" << endl;
	
	if(it == object_serverobj->buffered_msg.end())
	{
		std::vector<std::string> temp = it->second;
		temp.push_back(msg);
		it->second = temp;
	}
	else
	{
		//Add the ip to the map
		std::vector<std::string> temp;
		temp.push_back(msg);
		object_serverobj->buffered_msg.insert(std::make_pair(ip,temp));
		
	}
}

std::vector<std::string> server :: buf_recov_msg(string ip)
{
	int stat_cli = 0;
	std::vector<std::string> temp;
	server* object_serverobj = server::GetObject();
	int t_leng = 0; 
	std::map<std::string,std::vector<std::string> >::iterator it = object_serverobj->buffered_msg.find(ip);
	if(it != object_serverobj->buffered_msg.end())
	{	
		stat_cli = t_leng + 1;
		temp = it->second;
		std::vector<std::string> temp2 = it->second;
		temp2.clear();
		it->second = temp2;
		
	}
	else
	{
		cout<<"There are no buffered msgs available for this client"<<endl;
		cout << "you can try again" << endl;

	}
	return temp;
}
