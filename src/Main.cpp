

#include <iostream>
#include <stdio.h>
#include <string> 

#include "../include/global.h"
#include "../include/logger.h"
#include "../include/server.hpp"
#include "../include/client.hpp"

using namespace std;


int main(int argc, char **argv)
{
	// initialization of Logger
	cse4589_init_log(argv[2]);

	// Clearing the LOGFILE
    fclose(fopen(LOGFILE, "w"));

	// starting the process
	if(std::string(argv[1]) == "s")
	{
		server* serverObj = server::GetObject();
		serverObj->server_init(argv, argc );
	}
	else if(std::string(argv[1]) == "c")
	{
		client* obj_client = client::GetObject();
		obj_client->client_init(argv, argc);
	}
	return 0;
}

