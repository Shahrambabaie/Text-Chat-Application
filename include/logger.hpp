#ifndef LOGGER_H_
#define LOGGER_H_

#define length_file_path 512

extern char LOGFILE[length_file_path];

extern int ret_logg, ret_prnt;

void cse4589_print_and_log(const char* format, ...);
void cse4589_init_log(char* port);

#endif

