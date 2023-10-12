CC = gcc
CFLAGS=-Wall -Wextra


all:
	$(CC) $(CFLAGS) SimpleProgram.c L1Cache.c -o L1Cache


clean:
	rm L1Cache
	rm results.txt

t:
	make all
	./L1Cache > results.txt
	diff results.txt results_L1.txt
