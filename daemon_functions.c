#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "daemon_functions.h"

int getNextSeconds(SingleCommand cmd){
	time_t now = setNow();
	return (int)difftime(cmd.commandTime, now);
}

void runCommand(char* cmd, int info, int out_fd){
	char** args;
	char* bufor;
	int index = 1;

	bufor = strtok(cmd," ");
	while(bufor != NULL){
		bufor = strtok(NULL," ");
		index++;
	}
	args = (char**)malloc(sizeof(char*)*(index+1));
	index = 0;

	bufor = strtok(cmd," ");
	while(bufor != NULL){
		args[index] = (char*)malloc(sizeof(char)*strlen(bufor));
		strcpy(args[index++],bufor);
		bufor = strtok(NULL," ");
	}
	args[index] = NULL;

	pid_t pid;
	
	pid = fork();
	if(pid < 0){
		printf("Zesralo sie\n");
		exit(EXIT_FAILURE);
	}

	if(pid == (pid_t)0){
		dup2(out_fd,STDOUT_FILENO);
		//if(setsid() < 0)
		//	exit(EXIT_FAILURE);
		printf("Output of: ");
		for(int i = 0; i < index;i++){
			printf("%s ",args[i]);
		}
		printf("\n");
		execvp(args[0], args);
	}
	else{
	}

}

time_t setNow(void){
	time_t now;
	time(&now);
	return now;
}


