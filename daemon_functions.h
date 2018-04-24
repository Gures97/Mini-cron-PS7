#ifndef DAEMON_FUNCTIONS_H
#define DAEMON_FUNCTIONS_H
#include "command_list.h"
#include <stdlib.h>


int getNextSeconds(SingleCommand cmd);

void runCommand(char* cmd, int info, int out_fd);

time_t setNow(void);

#endif
