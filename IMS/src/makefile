PROGS = sim
CC = g++
CFLAGS = -std=c99 -Wall -o -lm -pedantic -Wextra
OFILES = sim.o lib.o

all: $(PROGS)
run: $(PROGS)
	./$(PROGS)

$(PROGS): $(OFILES)
	$(CC) $(CFLAGS) -o $(PROGS) $(OFILES)

%: %.c 
	$(CC) $(CFLAGS) "$@" "$<"

help:
	@echo "make zkompiluje celý projekt: ./sim"

clean:
	rm -f $(PROGS) *.o 