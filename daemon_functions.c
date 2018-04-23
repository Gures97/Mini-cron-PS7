#include <time.h>
#include <unistd.h>
#include <string.h>
#include "daemon_functions.h"
#include "command_list.h"

int getNextSeconds(SingleCommand cmd){
	time_t now = setNow();
	return (int)difftime(cmd.commandTime, now);
}

void runCommand(char* cmd, int info){
	char** args;
	char* bufor = strtok(command," ");
	int index = 0;
	while(bufor != NULL){
		args[index]=bufor;
		bufor = strtok(NULL," ");
	}
	pid_t pid;
	
	pid = fork();
	if(pid < 0){
		exit(EXIT_FAILURE);
	}

	if(pid == (pid_t)0){
		//proces potomny
		execvp(args[0], args[0]);
	}	
}

time_t setNow(void){
	time_t now;
	time(&now);
	return now;
}


