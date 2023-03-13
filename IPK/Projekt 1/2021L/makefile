PROGS = hinfosvc
CC = g++
CFLAGS = -std=c99 -Wall -o -lm -pedantic -Wextra
OFILES = main.o lib.o

all:$(PROGS) 

$(PROGS): $(OFILES)
	$(CC) $(CFLAGS) -o $(PROGS) $(OFILES)

%: %.c 
	$(CC) $(CFLAGS) "$@" "$<"

help:
	@echo "make 													zkompiluje celý projekt:./IPK-Projekt-1-2022"

clean:
	rm -f $(PROGS) *.o 