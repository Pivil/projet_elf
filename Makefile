CC=gcc

all: tElf

debug: d_tElf

printElf.o: printElf.c printElf.h
	$(CC) -c $^

main.o: main.c printElf.h
	$(CC) -c $^

tElf: main.o printElf.o
	$(CC) $^ -o $@

d_printElf.o: printElf.c printElf.h
	$(CC) -g -c $< -o $@

d_main.o: main.c printElf.h
	$(CC) -g -c $< -o $@

d_tElf: d_main.o d_printElf.o
	$(CC) -g $^ -o $@


