OBJ = main.o command_list.o daemon_functions.o

all: minicron

minicron: $(OBJ)
	gcc $(OBJ) -o minicron

$(OBJ): command_list.h daemon_functions.h

.PHONY: clean

clean:
	rm -f *.o
