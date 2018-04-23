#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include "command_list.h"

int main(int argc, char* argv[]){
	pid_t pid, sid;
	int zadania_fd, logi_fd;

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

	/*if((chdir("/")) < 0){
		exit(EXIT_FAILURE);
	}*/


	if(argc != 3) {
        printf("Usage: ./minicron <taskfile> <outfile>");
        exit(1);
	}


	int taskfile_fd = open(argv[1], O_RDONLY);
    if(taskfile_fd == -1) {
        perror(argv[1]);
        exit(2);
    }

    //close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	//close(STDERR_FILENO);

	CommandList* root = NULL;

	createCommandList(root, taskfile_fd);	
	printf("Succes\n");

/*	

	while(1){
		/* kod demona

		sleep(30);
	}
*/
    close(taskfile_fd);
	exit(EXIT_SUCCESS);
}
