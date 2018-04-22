#include <time.h>
#include <string.h>
#include "command_list.h"

void addCommand(CommandList* root
		int hour, 
		int minute, 
		char[] command, 
		int info,
		time_t now){

	struct tm commandTime;
	CommandList new = (SingleCommand)malloc(sizeof(SingleCommand));
	new->next = NULL;
	strcpy(new->command, command);
	
	commandTime = *localtime(&now);
	commandTime.tm_hour = hour;
	commandTime.tm_min = minute;
	if(difftime(mktime(&commandTime), now) <= 0)
		commandTime.tm_mday += 1;

	new->commandTime = commandTime;		
	if(*root == NULL)
		*root = new;
	else
		last(*root)->next = new;

}

SingleCommand getNext(CommandList* root){
	SingleCommand command;
	CommandList newRoot;
	sort(root);
	command = **root;

	newRoot = (*root)->next;
	free(*root);
	*root = newRoot;
}

void clear(CommandList* root){

}

void deleteCommand(CommandList* root, int commandIndex){
	CommandList temp = *root,prev = NULL;

	for(int i = 0; i<commandIndex;i++){
		if(temp == NULL)
			return;
		prev = temp;
		temp = temp->next;
	}
	if(temp == *root)
		*root = temp->next;
	else
		prev->next = temp->next;
	free(temp);
}

CommandList last(CommandList root){
	if(root == NULL || root->next == NULL)
		return root;
	else
		return last(root->next);
}

void rotate(CommandList* root){
	CommandList newRoot = (*root)->next;
	CommandList last = last(*root);
	
	last->next = *root;
	(*root)->next = NULL;
	*root = newRoot;
}
