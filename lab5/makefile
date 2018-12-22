COMP = gcc

all: lab5

lab5: bmp.o
	$(COMP) -o lab5 bmp.o

bmp.o: bmp.c
	$(COMP) -c bmp.c

clean:
	rm -f lab5 bmp.o
