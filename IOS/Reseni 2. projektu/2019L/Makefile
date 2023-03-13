# IOS - Project 2
# Author: Samuel Kuchta

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
LFLAGS=-lpthread
BIN=proj2
SOURCE=proj2.c

all:
	$(CC) $(CFLAGS) $(SOURCE) -g -o $(BIN) $(LFLAGS)

run: all
	./$(BIN)

clean:
	rm $(BIN)
