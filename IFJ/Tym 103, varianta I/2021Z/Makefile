PROGS = IFJ-Projekt-2021
CC = gcc
CFLAGS = -std=c99 -Wall -g -lm -pedantic -Wextra
OFILES = main.o syntakticka_analyza.o SA_tree.o lexikalni_analyza.o knihovny.o generator_mezikodu.o semanticka_analyza.o symtable.o

all:$(PROGS) 

$(PROGS): $(OFILES)
	$(CC) $(CFLAGS) -o $(PROGS) $(OFILES)

%: %.c 
	$(CC) $(CFLAGS) "$@" "$<"

help:
	@echo "make 													zkompiluje celý projekt:./IFJ-Projekt-2021"

clean:
	rm -f $(PROGS) *.o *-tests 
	rm -fr html 