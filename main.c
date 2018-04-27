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

<<<<<<< HEAD
CommandList cmdlist = NULL;
=======
CommandList cmdlist;
int taskfile_fd;
>>>>>>> master

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
<<<<<<< HEAD
	int taskfile_fd, outfile_fd, null_fd;
=======
	int outfile_fd;
>>>>>>> master

	SingleCommand nextCommand;
	cmdlist = NULL;


	pid = fork();
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

	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);

	pid = fork();

	if(pid < 0){
		exit(EXIT_FAILURE);
	}

	if(pid > 0){
		exit(EXIT_SUCCESS);
	}

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
	
	null_fd = open("/dev/null", O_WRONLY);
    	if(null_fd == -1) {
        	perror("/dev/null");
        	exit(2);
    	}

	openlog(NULL, LOG_NDELAY, LOG_INFO);

	createCommandList(&cmdlist, taskfile_fd);
	syslog(LOG_INFO, "Mini-cron started");
	char ***command = NULL;
	while(cmdlist != NULL){
		nextCommand = getNext(cmdlist);
		while(getNextSeconds(nextCommand) > 0)
			sleep(getNextSeconds(nextCommand));
		runCommand(nextCommand.command,0,outfile_fd,null_fd);
		deleteRoot(&cmdlist);
	}
	
	close(taskfile_fd);
	close(outfile_fd);
	syslog(LOG_INFO,"All done, good bye");
	closelog();
	exit(EXIT_SUCCESS);
}
