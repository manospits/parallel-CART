OBJS   = main.o tree.o node.o sampling.o io.o list.o element.o
OUT    = cart
CC     = mpicc
FLAGS  = -g -c -Wall -std=c99

ALL: $(OUT) 
$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

main.o: main.c
	$(CC) $(FLAGS) $?

tree.o: tree/src/tree.c
	$(CC) $(FLAGS) $?

node.o: tree/src/node.c
	$(CC) $(FLAGS) $?

sampling.o: forest/src/sampling.c
	$(CC) $(FLAGS) $?

io.o: io/src/io.c
	$(CC) $(FLAGS) $?

list.o: data_structures/src/list.c
	$(CC) $(FLAGS) $?

element.o: data_structures/src/element.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(OBJS) $(OUT) forest_main.o


