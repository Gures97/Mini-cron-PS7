#include "command_list.h"

void addCommand(CommandList* root
		int hour, 
		int minute, 
		char[] command, 
		int info){
	CommandList new = (SingleCommand)malloc(sizeof(SingleCommand));
	new->next = NULL;
	new->hour = hour;
	new->minute = minute;
	strcpy(new->command, command);
	if(*root == NULL)
		*root = new;
	else
		last(*root) = new;

}

SingleCommand getNext(CommandList* root){
	
}

void clear(CommandList* root){

}

void deleteCommand(CommandList* root, int commandIndex){

}

CommandList last(CommandList root){
	if(root == NULL || root->next == NULL)
		return root;
	else
		return last(root->next);
}
