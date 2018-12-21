COMP = gcc
.PHONY: all clean lab4

all: lab6

lab6: mem.o debug.o 
	$(COMP) -o lab6 mem.o debug.o

mem.o: mem.c
	$(COMP) -c mem.c

debug.o: debug.c
	$(COMP) -c debug.c

lab4: mem.o list.o main.o
	$(COMP) -o lab4 mem.o list.o main.o

list.o: list.c
	$(COMP) -c -o list.o list.c

main.o: main.c
	$(COMP) -c main.c

clean:
	rm -f lab6 lab4 mem.o debug.o list.o main.o
