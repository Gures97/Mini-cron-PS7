#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include "command_list.h"
#include "daemon_functions.h"

CommandList cmdlist = NULL;

void handler(int signum){
	switch(signum){
		case SIGUSR1:
		break;
		case SIGUSR2:
			saveToSyslog(cmdlist);
		break;
	}
}

int main(int argc, char* argv[]){
	pid_t pid, sid;
	int taskfile_fd, outfile_fd;
	char komenda[] = "ls";

	SingleCommand nextCommand;
	
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	/*pid = fork();
	if(pid < 0){
		exit(EXIT_FAILURE);
	}

	if(pid > 0){
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();
	if(sid < 0){
		exit(EXIT_FAILURE);
	}

	/*if((chdir("/")) < 0){
		exit(EXIT_FAILURE);
	}*/


	if(argc != 3) {
        printf("Usage: ./minicron <taskfile> <outfile>");
        exit(1);
	}

	taskfile_fd = open(argv[1], O_RDONLY);
    	if(taskfile_fd == -1) {
        	perror(argv[1]);
        	exit(2);
    	}

	outfile_fd = open(argv[2], O_WRONLY | O_APPEND);
    	if(outfile_fd == -1) {
        	perror(argv[2]);
        	exit(2);
    	}
	
	createCommandList(&cmdlist, taskfile_fd);	

	while(cmdlist != NULL){
		nextCommand = getNext(&cmdlist);
		sleep(getNextSeconds(nextCommand));
		runCommand(nextCommand.command,0,outfile_fd);
	}
	
	close(taskfile_fd);
	close(outfile_fd);
	exit(EXIT_SUCCESS);
}
