CC = gcc
CFLAGS=-Wall -Wextra


all:
	$(CC) $(CFLAGS) SimpleProgram.c L2Cache_2W.c -o L2Cache_2W

clean:
	rm L2Cache_2W
	rm results.txt

t:
	make all
	./L2Cache_2W > results.txt
	diff results.txt results_L2_2W.txt
