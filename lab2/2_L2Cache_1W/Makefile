CC = gcc
CFLAGS=-Wall -Wextra


all:
	$(CC) $(CFLAGS) SimpleProgram.c L2Cache_1W.c -o L2Cache_1W

clean:
	rm L2Cache_1W
	rm results.txt

t:
	make all
	./L2Cache_1W > results.txt
	diff results.txt results_L2_1W.txt
