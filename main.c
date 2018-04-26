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

CommandList cmdlist;
int taskfile_fd;

void handler(int signum){
	switch(signum){
		case SIGUSR1:
			clearList(&cmdlist);
			createCommandList(&cmdlist, taskfile_fd);
		break;
		case SIGUSR2:
			saveToSyslog(cmdlist);
		break;
	};
}

int main(int argc, char* argv[]){
	pid_t pid, sid;
	int outfile_fd;

	SingleCommand nextCommand;
	cmdlist = NULL;
	
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

	openlog(NULL, LOG_NDELAY, LOG_INFO);

	createCommandList(&cmdlist, taskfile_fd);
	raise(SIGUSR2);
	
	while(cmdlist != NULL){
		nextCommand = getNext(cmdlist);
		sleep(getNextSeconds(nextCommand));
		runCommand(nextCommand.command,0,outfile_fd);
		deleteRoot(&cmdlist);
	}
	
	close(taskfile_fd);
	close(outfile_fd);
	syslog(LOG_INFO,"All done, good bye");
	closelog();
	exit(EXIT_SUCCESS);
}
