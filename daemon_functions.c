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

