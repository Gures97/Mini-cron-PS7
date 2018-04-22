#include <time.h>
#include <unistd.h>
#include <string.h>
#include "daemon_functions.h"
#include "command_list.h"

int getNextSeconds(SingleCommand cmd, time_t now){
	return (int)difftime(cmd.commandTime, now);
}

void runCommand(SingleCommand cmd){
	char* command = cmd.command;
	char** args;
	char* bufor = strtok(command," ");
	while(bufor != NULL){
		args
	}
	pid_t pid, sid;
	
	pid = fork();
	if(pid < 0){
		exit(EXIT_FAILURE);
	}

	if(pid == (pid_t)0){
		//proces potomny
		execvp(args[0], args[0]);
	}

	umask(0);

	sid = setsid();
	if(sid < 0){
		exit(EXIT_FAILURE);
	}

	if((chdir("/")) < 0){
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(SDROUT_FILENO);
	close(STDERR_FILENO);
	
}

time_t setNow(void){
	time_t = now;
	time(&now);
	return now;
}


