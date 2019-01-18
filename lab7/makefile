COMP = gcc
ASM = nasm

all: lab7

lab7: main.c sepia_filter.o sepia_sse.o bmp.o
	$(COMP) -o lab7 -g main.c sepia_filter.o sepia_sse.o bmp.o

sepia_filter.o: sepia_filter.c
	$(COMP) -c -g -o sepia_filter.o sepia_filter.c

sepia_sse.o: sepia_sse.asm
	$(ASM) -fELF64 -o sepia_sse.o sepia_sse.asm

bmp.o: bmp.c
	$(COMP) -o bmp.o -c bmp.c

clean:
	rm -f lab7 *.o
