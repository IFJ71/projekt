CFLAGS=-std=c99 -Wall -pedantic -g
BIN=project
CC=gcc
RM=rm -f

ALL: str.o scanner.o main.o ial.o parser.o
	$(CC) $(CFLAGS) -o $(BIN) str.o scanner.o main.o ial.o parser.o

clean:
	$(RM) *.o $(BIN)
