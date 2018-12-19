CC=clang -std=c99 -Wall -Wextra -Wfatal-errors -fdiagnostics-color=auto


exec = printELF
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
















# #The programs#
# printELFHeader : header.o
# 	$(CC) -o printArraySection arraySection.o
#
# printSectionHeader : sectionHeader.o header.o
# 	$(CC) -o printArraySection arraySection.o header.o
#
# printSection : section.o arraySection.o
# 	$(CC) -o printSection section.o arraySection.o
#
#
#
#
# #The object files#
# ELFheader.o : ELFheader.c ELFheader.h elf.h
# 	$(CC) -c header.c
#
# arraySection.o : arraySection.c arraySection.h elf.h
# 	$(CC) -c arraySection.c
#
# section.o : section.c section.h elf.h
# 	$(CC) -c section.c
#
#
#
#
# byteManipulation.o : byteManipulation.c byteManipulation.c
# 	$(CC) -c byteManipulation.c
#
#
#
# #The obect files for programs#
# printELFHeader.o : printELFHeader.c section.h arraySection.h elf.h
# 	$(CC) -c ELFheader.c
#
# printSectionHeader.o : printArraySection.c section.h arraySection.h elf.h
# 	$(CC) -c sectionHeader.c
#
#
# printSection.o : printSection.c section.h arraySection.h elf.h
# 	$(CC) -c printSection.c
