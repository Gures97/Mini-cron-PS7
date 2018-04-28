#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include "command_list.h"

void addCommand(CommandList* root,
		int hour,
		int minute,
		char* command,
		int info,
		time_t now){

	struct tm * commandTime;
	CommandList tail = last(*root);
	CommandList new = (CommandList)malloc(sizeof(SingleCommand));

	new->next = NULL;
	new->command = command;
	new->info = info;
	commandTime = localtime(&now);
	commandTime->tm_hour = hour;
	commandTime->tm_min = minute;
	commandTime->tm_sec = 0;
	if(difftime(mktime(commandTime), now) <= 0)
		commandTime->tm_mday += 1;

	new->commandTime = mktime(commandTime);
	if(*root == NULL)
		*root = new;
	else{
		tail->next = new;
	}

}

void createCommandList(CommandList* root, int taskfile_fd)
{
	char ch, *pch, *command;
    	char* buffer = NULL;
	char* buffbuffer = NULL;
	time_t now;
	int i, hour, minutes, info, isEnd;

    	do
    	{
        	i=0;
        	while(1){
			isEnd = read(taskfile_fd, &ch, 1);
            		
			i++;
            		buffbuffer = (char*)realloc(buffer, i * sizeof(char));
			if(buffbuffer != NULL){
				buffer = buffbuffer;
				if(ch == '\n' || ch == 0){
					buffer[i-1] = '\0';
					break;
				}
				buffer[i-1] = ch;
			}
			else if(buffbuffer == NULL){
				syslog(LOG_INFO, "cmdlist creation error: buffer error");
				closelog();
				exit(EXIT_FAILURE);
			}

		}

		if(strlen(buffer) > 0){
			pch = strtok (buffer, ":");
			if((hour = atoi(pch)) < 0){
				syslog(LOG_INFO, "cmdlist creation error: invalid 'hour'");
				closelog();
				exit(EXIT_FAILURE);
			}
			pch = strtok (NULL, ":");
			if((minutes = atoi(pch)) < 0){
				syslog(LOG_INFO, "cmdlist creation error: invalid 'minutes'");
				closelog();
				exit(EXIT_FAILURE);
			}
			pch = strtok (NULL, ":");
			command = (char*)malloc(sizeof(char)*strlen(pch));
			strcpy(command, pch);
			pch = strtok (NULL, ":");
			if((info = atoi(pch)) < 0){
				syslog(LOG_INFO, "cmdlist creation error: invalid 'info'");
				closelog();
				exit(EXIT_FAILURE);
			}
	    		time(&now);
			addCommand(root, hour, minutes, command, info, now);
		}

    	}while(isEnd != 0);
	free(buffer);
	sort(*root);
}

void updateCommandList(CommandList* root, char* taskfile){
	clearList(root);
	int taskfile_fd = open(taskfile, O_RDONLY);
	if(taskfile_fd == -1) {
		perror(taskfile);
		exit(2);
	}
	createCommandList(root, taskfile_fd);
	//close(taskfile_fd);
}

void deleteRoot(CommandList* root){
	CommandList newRoot;
	newRoot = (*root)->next;
	free(*root);
	*root = newRoot;
}

SingleCommand getNext(CommandList root){
	return *root;
}

void clearList(CommandList* root){
	while(*root != NULL)
		deleteRoot(root);
}

CommandList last(CommandList root){
	if(root == NULL)
		return NULL;
	while(root->next != NULL)
		root = root->next;
	return root;
}

void sort(CommandList root){
	CommandList temp = root->next;
	CommandList earliest = root;
	while(root->next != NULL){
		while(temp != NULL){
			if(temp->commandTime < earliest->commandTime)
				earliest = temp;
			temp = temp->next;
		}
		if(root != earliest)
			swap(root,earliest);
		root = root->next;
		temp = root->next;
		earliest = root;
	}
}

void swap(CommandList a, CommandList b){
	SingleCommand temp;
	temp.commandTime = a->commandTime;
	temp.command = a->command;
	temp.info = a->info;

	a->commandTime = b->commandTime;
	a->command = b->command;
	a->info = b->info;

	b->commandTime = temp.commandTime;
	b->command = temp.command;
	b->info = temp.info;
}
