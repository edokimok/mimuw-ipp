CC = gcc
CFLAGS = -Wall -Wextra -g -Wno-implicit-fallthrough -std=c17 -O2
LDFLAGS =

.PHONY: all clean

all: labyrinth

labyrinth: queue.o parse_input.o bfs.o main.o
	$(CC) $(LDFLAGS) -o $@ $^

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c $<

parse_input.o: parse_input.c parse_input.h queue.h
	$(CC) $(CFLAGS) -c $<

bfs.o: bfs.c bfs.h parse_input.h queue.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c bfs.h parse_input.h queue.h
	$(CC) $(CFLAGS) -c $<


clean:
	-rm -f *.o labyrinth



