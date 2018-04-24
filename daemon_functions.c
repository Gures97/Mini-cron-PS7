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
	printf("runCommand\n");
	char** args;
	char* bufor;
	int index = 1;

	bufor = strtok(cmd," ");
	while(bufor != NULL){
		bufor = strtok(NULL," ");
		index++;
	}
	args = (char**)malloc(sizeof(char*)*index);
	index = 0;

	bufor = strtok(cmd," ");
	while(bufor != NULL){
		args[index] = (char*)malloc(sizeof(char)*strlen(bufor));
		strcpy(args[index],bufor);
		bufor = strtok(NULL," ");
	}

	printf("porobione argsy przed forkiem\n");
	pid_t pid;
	
	pid = fork();
	if(pid < 0){
		printf("Zesralo sie\n");
		exit(EXIT_FAILURE);
	}

	if(pid == (pid_t)0){
		printf("leci proces\n");
		printf("Args[0]: %s", args[0]);
		printf("niepowinno");
		dup2(out_fd,STDOUT_FILENO);
		execvp(args[0], args);
		printf("Wyleciala plyta");
		exit(EXIT_SUCCESS);
	}
	else{
		printf("czekam\n");
	}
}

time_t setNow(void){
	time_t now;
	time(&now);
	return now;
}


