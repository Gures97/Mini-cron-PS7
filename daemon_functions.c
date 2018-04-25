#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "daemon_functions.h"
#include <syslog.h>


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

void saveToSyslog(CommandList root){
	openlog(NULL, LOG_NDELAY, LOG_INFO);
	while(root != NULL) {
	
		struct tm commandTime;
		localtime_r(&(root->commandTime), &commandTime);
		time_t now;
		time(&now);
		struct tm currentTime;
		localtime_r(&now, &currentTime);
		printf("Command Time: %d:%d %d.%d.%d\n", commandTime.tm_hour, commandTime.tm_min, commandTime.tm_mday, commandTime.tm_mon, commandTime.tm_year);
		if(commandTime.tm_mday == currentTime.tm_mday &&	
			(commandTime.tm_hour > currentTime.tm_hour ||
			commandTime.tm_hour == currentTime.tm_hour && commandTime.tm_min > currentTime.tm_hour))
			 {
			syslog(LOG_INFO,"%d:%d:%s:%d", commandTime.tm_hour, commandTime.tm_min, root->command, root->info);
		}
		root = root->next;
	}
	closelog();
}

