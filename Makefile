CC=clang -std=c99 -Wall -Wextra -Wfatal-errors -fdiagnostics-color=auto
.PHONY: clean

exec = printELF relocTest
all:$(exec)


printELF : printELF.o readELF.o byteManipulation.o
	$(CC) -g -o $@ $^

printELF.o : printELF.c readELF.h elf.h
	$(CC) -g -c $<

readELF.o : readELF.c readELF.h byteManipulation.h elf.h
	$(CC) -g -c readELF.c

byteManipulation.o : byteManipulation.c byteManipulation.c
	$(CC) -g -c byteManipulation.c




relocTest: relocTest.o relocation.o readELF.o byteManipulation.o
	$(CC) -g $^ -o $@

relocTest.o: relocTest.c relocation.h
	$(CC) -g -c $<

relocation.o: relocation.c relocation.h readELF.h
	$(CC) -g -c $<

clean:
	rm -f *.o relocTest printELF
