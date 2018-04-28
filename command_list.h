#ifndef COMMANDLIST_H
#define COMMANDLIST_H
#include <time.h>
typedef struct singlecommand{
	time_t commandTime;
	char* command;
	int info;
	struct singlecommand *next;
} SingleCommand;

typedef SingleCommand *CommandList;

void addCommand(CommandList* root,
		int hour,
		int minute,
		char* command,
		int info,
		time_t now);

SingleCommand getNext(CommandList root);

void deleteRoot(CommandList* root);

void clearList(CommandList* root);

CommandList last(CommandList root);

void createCommandList(CommandList* root, int taskfile_fd);

void updateCommandList(CommandList* root, char* taskfile);

void swap(CommandList a, CommandList b);

void sort(CommandList root);
#endif
