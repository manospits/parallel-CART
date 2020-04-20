OBJS   = main.o tree.o io.o
SOURCE = main.c tree.c io.c
INCLUDE_PATH='./include'
HEADER = ${INCLUDE_PATH}/tree.h 
OUT    = cart
CC     = gcc
FLAGS  = -g -c -Wall

ALL: $(OUT) 
$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@


main.o: main.c
	$(CC) $(FLAGS) $?

tree.o: tree.c
	$(CC) $(FLAGS) $?

io.o: io.c
	$(CC) $(FLAGS) $?

element.o: element.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(OBJS) $(OUT)


count:
	wc $(SOURCE) $(header)
