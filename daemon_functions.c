#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "daemon_functions.h"
#include <syslog.h>
#include <sys/wait.h>


int getNextSeconds(SingleCommand cmd){
	time_t now = setNow();
	return (int)difftime(cmd.commandTime, now);
}

void runCommand(char* cmd, int info, int out_fd){
	char** args;
	char* bufor;
	char* fullCommand = (char*)malloc(strlen(cmd)*sizeof(char));
	int index = 1;
	char header[] = "\nOutput of: ";
	char spacer[] = "\n\n";

	strcpy(fullCommand, cmd);
	bufor = strtok(fullCommand," ");
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
		syslog(LOG_INFO,"Fork error: %s", args[0]);
		closelog();
		exit(EXIT_FAILURE);
	}

	if(pid == (pid_t)0){
		syslog(LOG_INFO, "Execute command %s", args[0]);
		dup2(out_fd,STDOUT_FILENO);
		write(out_fd,header,strlen(header)*sizeof(char));
		for(int i = 0; i < index;i++){
			write(out_fd,args[i],strlen(args[i])*sizeof(char));
		}
		write(out_fd,spacer,strlen(spacer) * sizeof(char));
		execvp(args[0], args);
	}
	else{
		waitpid(pid,NULL,0);
	}

}

time_t setNow(void){
	time_t now;
	time(&now);
	return now;
}

void saveToSyslog(CommandList root){
	syslog(LOG_INFO,"List of commands");
	while(root != NULL) {
		struct tm * commandTime = localtime(&(root->commandTime));
		
		syslog(LOG_INFO,"%d:%d:%s:%d", commandTime->tm_hour, commandTime->tm_min, root->command, root->info);
		root = root->next;
	}
}

