
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <iostream>


#include "../include/global.h"
#include "../include/logger.h"

using namespace std;

char LOGFILE[length_file_path];
void cse4589_init_log(char* port)
{
	// obtaining the hostname and creating file paths
	FILE* fp;
	fp = popen("echo $HOSTNAME | tr '.' '\n' | sed -n 1p", "r"); //Gets the local hostname (without the domain name part)
	if (fp == NULL) {
    	cout << "There is a problem in obtaing the hostname!\n" << endl;
		cout << " You can try another time!\n" << endl;

		exit(1);
	}

	char* hostname = (char*) malloc(len_host*sizeof(char));
	bzero(hostname, len_host);
	fscanf(fp, "%s[^\n]", hostname);

	// obtaining the executable's path*
	char* exec_path = (char*) malloc(length_path*sizeof(char));
	bzero(exec_path, length_path);
  	if (readlink("/proc/self/exe", exec_path, length_path) == -1) 
	{
    	cout << "There is a problem in obtaing the executable path!\n" << endl;
		cout << " You can try another time!\n" << endl;
		
	}

	char* exec_dir = (char*) malloc(length_path*sizeof(char));
	bzero(exec_dir, length_path);
	memcpy(exec_dir, exec_path, strlen(exec_path)-12);
	free(exec_path);

	bzero(LOGFILE, length_file_path);
	sprintf(LOGFILE, "%s/logs/assignment_log_%s_%s", exec_dir, hostname, port);

	free(exec_dir);
	free(hostname);
	fclose(fp);
}

int ret_logg, ret_prnt;
void cse4589_print_and_log(const char* format, ...)
{
	va_list args_pointer;

	va_start(args_pointer, format);
   	ret_prnt = vprintf(format, args_pointer);

	// Write to the LOG File
	FILE* fp;
	if((fp = fopen(LOGFILE, "a")) == NULL){
		ret_logg = -100;
		//cleaning before exit
		va_end(args_pointer);
	}

	va_start(args_pointer, format);
	ret_logg = vfprintf(fp, format, args_pointer);

	fclose(fp);
    va_end(args_pointer);
}

