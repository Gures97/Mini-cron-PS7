#ifndef COMMANDLIST_H
#define COMMANDLIST_H

typedef struct singlecommand{
	int hour;
	int minute;
	char[] command;
	int info;
	struct singlecommand *next;
} SingleCommand;

typedef SingleCommand *CommandList;

void addCommand(CommandList* root, 
		int hour, 
		int minute
		char* command, 
		int info);

SingleCommand getNext(CommandList* root);

void clear(CommandList* root);

void deleteCommand(CommandList* root, 
		int commandIndex);

#endif
