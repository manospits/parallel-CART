OBJS   = main.o tree.o node.o forest.o io.o
OUT    = cart
CC     = gcc
FLAGS  = -g -c -Wall

ALL: $(OUT) 
$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@


main.o: main.c
	$(CC) $(FLAGS) $?

tree.o: tree/src/tree.c
	$(CC) $(FLAGS) $?

node.o: tree/src/node.c
	$(CC) $(FLAGS) $?

forest.o: forest/src/forest.c
	$(CC) $(FLAGS) $?

io.o: io/src/io.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(OBJS) $(OUT)


