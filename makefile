OBJ = main.o command_list.o daemon_functions.o

all: Mini-cron

Mini-cron: $(OBJ)
	gcc $(OBJ) -o Mini-cron

$(OBJ): command_list.h daemon_functions.h

.PHONY: clean

clean:
	rm -f *.o
