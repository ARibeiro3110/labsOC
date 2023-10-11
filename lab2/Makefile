CC = gcc
CFLAGS=-Wall -Wextra


all:
	$(CC) $(CFLAGS) SimpleProgram.c L1Cache.c -o L1Cache
	$(CC) $(CFLAGS) SimpleProgram.c L2Cache_1W.c -o L2Cache_1W
	$(CC) $(CFLAGS) SimpleProgram.c L2Cache_2W.c -o L2Cache_2W

clean:
	rm L1Cache
	rm L2Cache_1W
	rm L2Cache_2W
	rm results.txt

t:
	make all
	./L1Cache > results.txt
	diff results.txt results_L1.txt
	./L2Cache_1W > results.txt
	diff results.txt results_L2_1W.txt
	./L2Cache_2W > results.txt
	diff results.txt results_L2_2W.txt
	make clean
