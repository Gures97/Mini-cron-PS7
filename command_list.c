#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "command_list.h"

void addCommand(CommandList* root,
		int hour,
		int minute,
		char* command,
		int info,
		time_t now){

	struct tm commandTime;
	CommandList tail = last(*root);
	CommandList new = (CommandList)malloc(sizeof(SingleCommand));
	new->next = NULL;
	strcpy(new->command, command);

	commandTime = *localtime(&now);
	commandTime.tm_hour = hour;
	commandTime.tm_min = minute;
	if(difftime(mktime(&commandTime), now) <= 0)
		commandTime.tm_mday += 1;

	new->commandTime = mktime(&commandTime);
	if(*root == NULL)
		*root = new;
	else
		tail->next = new;

}

void createCommandList(CommandList* root, int taskfile_fd)
{
    char ch, *pch;
    char buffer[60];
	int i;
	for(i=0; i<60; i++) {
		buffer[i] = '0';
	};

    do
    {
        i=0;
        do
        {
            read(taskfile_fd, &ch, 1);
            buffer[i++] = ch; 
	} while(ch != '\n' || ch != EOF);
/*
	pch = strtok (buffer, ":");
	int hour = atoi(pch);
	pch = strtok (NULL, ":");
	int minutes = atoi(pch);
	pch = strtok (NULL, ":");
	char *command;
	strcpy(command, pch);
	pch = strtok (NULL, ":");
	int info = atoi(pch);
	time_t now;
    time(&now);
	addCommand(root, hour, minutes, command, info, now);*/
    }while(ch != EOF);
}

SingleCommand getNext(CommandList* root){
	SingleCommand command;
	CommandList newRoot;
	command = **root;

	newRoot = (*root)->next;
	free(*root);
	*root = newRoot;
}

void clearList(CommandList* root){
	while(*root != NULL)
		getNext(root);
}

CommandList last(CommandList root){
	if(root == NULL)
		return NULL;
	while(root->next != NULL)
		root = root->next;
	return root;
}

int howMuchCommands(CommandList* root){
	int count = 0;
	CommandList temp = *root;
	while(temp != NULL){
		count++;
		temp = temp->next;
	}
	return count;
}
/*
void sort(CommandList* root){
	CommandList *ptr, *tmp_ptr = NULL;

	do
	{
		ptr = root;
		while(ptr->next != tmp_ptr) {
			if(difftime(ptr->commandTime, (ptr->next)->commandTime) < 0) {



}*/
