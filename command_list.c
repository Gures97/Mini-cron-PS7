#include <time.h>
#include <string.h>
#include <stdlib.h>
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
