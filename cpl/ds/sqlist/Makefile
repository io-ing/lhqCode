CC = gcc
CFLAGS = -Wall

all: main

main: main.o sqlist.o
	$(CC) $(CFLAGS) -o main main.o sqlist.o

main.o: main.c sqlist.h
	$(CC) $(CFLAGS) -c main.c

sqlist.o: sqlist.c sqlist.h
	$(CC) $(CFLAGS) -c sqlist.c

clean:
	rm -f main main.o sqlist.o

