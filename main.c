#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
//#include <string.h>

struct Task
{
    int hour, minutes;
    char command[50];
    int info;
    TAILQ_ENTRY(task) tasks;
};

TAILQ_HEAD(, Task) head;

void readTaskfile(int taskfile_fd)
{
    TAILQ_INIT(&head);
    char ch = '0';
    char buffer[60];
    struct Task *item;

    while(ch != EOF)
    {
        int i=0, j=-1;
        do
        {
            read(taskfile_fd, &ch, 1);
            buffer[i++] = ch;
        } while(ch != '\n');
        item = malloc (sizeof(*item));
        item->hour = buffer[0]*10 + buffer[1];
        item->minutes = buffer[3]*10+buffer[4];
        i = 5;
        while(buffer[i] != ':')
            item->command[++j] = buffer[i++];
        item->info = buffer[++i];
        TAILQ_INSERT_TAIL(&head, item, tasks);

    }
}

int main(int argc, char* argv[]){
	pid_t pid, sid;
	int zadania_fd, logi_fd;

	pid = fork();
	if(pid < 0){
		exit(EXIT_FAILURE);
	}

	if(pid > 0){
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();
	if(sid < 0){
		exit(EXIT_FAILURE);
	}

	if((chdir("/")) < 0){
		exit(EXIT_FAILURE);
	}


	if(argc != 3) {
        perror("Usage: ./minicron <taskfile> <outfile>");
        exit(1);
	}


	int taskfile_fd = open (argv[1], O_RDONLY);
    if(taskfile_fd == -1) {
        perror(argv[1]);
        exit(2);
    }

    close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	readTaskfile(taskfile_fd);

	while(1){
		/* kod demona*/

		sleep(30);
	}

    close(taskfile_fd);
	exit(EXIT_SUCCESS);
}
