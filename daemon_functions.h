#ifndef DAEMON_FUNCTIONS_H
#define DAEMON_FUNCTIONS_H

int getNextSeconds(SingleCommand cmd);

void runCommand(SingleCommand cmd);

time_t setNow(void);

#endif
