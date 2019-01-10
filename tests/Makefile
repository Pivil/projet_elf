CC=arm-eabi-gcc
CCE=arm-elf-gcc
AS=arm-eabi-as
LD=arm-eabi-ld
OPT_BIG=-mbig-endian -mno-thumb-interwork -g -O2
OPT_EX=-mbig-endian -mno-thumb-interwork -g -O2 -nostdlib --entry main -n -Wl,--section-start -Wl,.text=0x20 -Wl,--section-start -Wl,.data=0x2800 -Wl,-EB
OPT_C_TO_S=-mno-thumb-interwork -S
.PHONY: clean

fich= $(shell ls *.s) #Récupère tous les .s
fichObj= $(patsubst %.s, %.o, $(fich)) #Tous les fichiers objets
fichEX= $(patsubst %.s, %, $(fich)) #Les exec correspondant

all:appelFonctionSimple example1 example2 example3 example4 exempleSimpleR_ARM_ABS8 exempleSimpleR_ARM_ABS16 exempleSimpleR_ARM_CALL exempleSimpleR_ARM_JUMP24 file_exe

################################################################################
#							Les exécutables
################################################################################
appelFonctionSimple : appelFonctionSimple.o
	$(CC) $(OPT_EX) -o $@ $<

example1 : example1.o
	$(CC) $(OPT_EX) -o $@ $<

example2 : example2.o
	$(CC) $(OPT_EX) -o $@ $<

example3 : example3.o
	$(CC) $(OPT_EX) -o $@ $<

example4 : example4.o
	$(CC) $(OPT_EX) -o $@ $<

exempleSimpleR_ARM_ABS8 : exempleSimpleR_ARM_ABS8.o
	$(CC) $(OPT_EX) -o $@ $<

exempleSimpleR_ARM_ABS16 : exempleSimpleR_ARM_ABS16.o
	$(CC) $(OPT_EX) -o $@ $<

exempleSimpleR_ARM_CALL : exempleSimpleR_ARM_CALL.o
	$(CC) $(OPT_EX) -o $@ $<

exempleSimpleR_ARM_JUMP24 : exempleSimpleR_ARM_JUMP24.o
	$(CC) $(OPT_EX) -o $@ $<




################################################################################
#							Test avec 2 fichiers
################################################################################
file_1.s: file_1.c
	$(CC) $(OPT_C_TO_S) file_1.c

file.s: file.c
	$(CC) $(OPT_C_TO_S) file.c

file.o: file.s
	$(AS) -o file.o file.s

file_1.o: file_1.s
	$(AS) -o file_1.o file_1.s

file_exe.o: file_1.o file.o
	$(LD) -r -o file_exe.o file_1.o file.o

file_exe: file_exe.o
	$(CC) -o file_exe file_exe.o

################################################################################
#							Les fichiers objets
################################################################################
%.o : %.s
	$(CC) $(OPT_BIG) -c -o $@ $<


clean :
	rm -f *.o $(fichEX) file_exe file_exe.o
