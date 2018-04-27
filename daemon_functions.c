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

void runCommand(char* cmd, int info, int out_fd, int null_fd){
	char*** cmds;
	char** currArg = NULL;
	char header[] = "\nOutput of: ";
	int status;
	int potok[2];
	int index = 0;
	int in_fd = STDIN_FILENO;

	cmds = sliceCommands(cmd);
	currArg = cmds[0];

	pid_t pid;
	syslog(LOG_INFO, "Execute command %s Number of commands: %i", cmd, countCMDs(cmds));
	while (currArg != NULL)
    	{
		pipe(potok);
		pid = fork();
		if(pid < 0){
			syslog(LOG_INFO, "Fork error: %s", currArg[0]);
			closelog();
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			dup2(in_fd, STDIN_FILENO);
			if(cmds[index+1] != NULL){
				dup2(potok[1],STDOUT_FILENO);
			}
			else{
				switch(info){
					case 0:
						dup2(out_fd,STDOUT_FILENO);
						dup2(null_fd,STDERR_FILENO);
						break;
					case 1:
						dup2(null_fd,STDOUT_FILENO);
						dup2(out_fd,STDERR_FILENO);
						break;
					case 2:
						dup2(out_fd,STDOUT_FILENO);
						dup2(out_fd,STDERR_FILENO);
						break;
				}
				write(out_fd,header,strlen(header)*sizeof(char));
				write(out_fd,cmd,strlen(cmd)*sizeof(char));
			}
			close(potok[0]);
			execvp(currArg[0], currArg);
			exit(EXIT_FAILURE);
		}
		else{
			waitpid(pid,&status,0);
			close(potok[1]);
			in_fd = potok[0];
			if(cmds[++index] == NULL)
				syslog(LOG_INFO, "Command %s executed, status: %i", currArg[0], status);
			currArg = cmds[index];
		}
	}
	flushCommand(&cmds);
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

char*** sliceCommands(char* cmd){
	char*** slices = NULL;	
	char* buffer;
	char** rawCommands = NULL;
	char** commandBuffer = NULL;
	int cmdCount = 0, index;

	buffer = strtok(cmd,"|");

	while(buffer != NULL){
		if(strlen(buffer) > 0){
			commandBuffer = (char**)realloc(rawCommands,(cmdCount+1) * sizeof(char*));
			rawCommands = commandBuffer;
			rawCommands[cmdCount] = (char*)malloc(strlen(buffer)*sizeof(char));
			strcpy(rawCommands[cmdCount],buffer);
			cmdCount++;
		}
syslog(LOG_INFO,"%s",buffer);
		buffer = strtok(NULL,"|");
	}
	rawCommands = (char**)realloc(rawCommands,(cmdCount+1) * sizeof(char*));
	rawCommands[cmdCount] = NULL;
	slices = (char***)malloc(sizeof(char**)*(cmdCount+1));
	for(int i = 0; i<cmdCount;i++)
		slices[i] = NULL;
	cmdCount = 0;
	while(rawCommands[cmdCount] != NULL){
		index = 0;
		buffer = strtok(rawCommands[cmdCount]," ");
		while(buffer != NULL){
			if(strlen(buffer)>0){
				commandBuffer = (char**)realloc(slices[cmdCount], (cmdCount+1)*sizeof(char*));
				slices[cmdCount] = commandBuffer;
				slices[cmdCount][index] = (char*)malloc(strlen(buffer)*sizeof(char));
				strcpy(slices[cmdCount][index++], buffer);
			}
			buffer = strtok(NULL," ");
		}
		commandBuffer = (char**)realloc(slices[cmdCount], (cmdCount+1)*sizeof(char*));
		slices[cmdCount] = commandBuffer;
		slices[cmdCount][index] = NULL;
		cmdCount++;
	}
	slices[cmdCount] = NULL;
	return slices;
}

void flushCommand(char**** slices){
	for(int i = 0;(*slices)[i] != NULL;i++){
		for(int j = 0; (*slices)[i][j] != NULL; j++){
			free((*slices)[i][j]);
		}
		free((*slices)[i]);
	}
	free(*slices);
}

int countCMDs(char*** args){
	char** temp;
	int count = 0;
	do{
		temp = args[count];
		count++;
	}while(temp != NULL);
		
	return count;
}

