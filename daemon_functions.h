#ifndef DAEMON_FUNCTIONS_H
#define DAEMON_FUNCTIONS_H
#include "command_list.h"
#include <stdlib.h>


int getNextSeconds(SingleCommand cmd);

void runCommand(char* cmd, int info, int out_fd, int null_fd);

time_t setNow(void);

void saveToSyslog(CommandList root);

void flushCommand(char**** slices);

char*** sliceCommands(char* cmd);

int countCMDs(char*** args);

#endif
