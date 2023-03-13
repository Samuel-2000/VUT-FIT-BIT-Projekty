PROGS = flow
CC = g++
CFLAGS = -std=c99 -Wall -o -lm -pedantic -Wextra
OFILES = flow.o lib.o

all:$(PROGS) 

$(PROGS): $(OFILES)
	$(CC) $(CFLAGS) -o $(PROGS) $(OFILES) -lpcap

%: %.c 
	$(CC) $(CFLAGS) "$@" "$<"

help:
	@echo "make zkompiluje celý projekt:./flow"

clean:
	rm -f $(PROGS) *.o 