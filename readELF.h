#ifndef __readELF_h__
#define __readELF_h__

#include "elf.h"
#include <stdio.h>

/******************************************************************************/
                                /* ELF header */
/******************************************************************************/

/*
Entries: the file to read
Result: the header
Precondition : we are at the beginning of the file
*/
Elf32_Ehdr readELFHeader(FILE *f);

/*
Entries: the header
Result: print the header
*/
void printELFHeader(Elf32_Ehdr hd);


/******************************************************************************/
                            /* Section header */
/******************************************************************************/

typedef struct Elf32_Shdr_seq {
    Elf32_Shdr * tab;
    int n;
} Elf32_Shdr_seq;

/*
Entries: the file to read, the header of the file
Result: the section header
The program allocates memory for the array
*/
Elf32_Shdr_seq readSectionHeader(FILE *f, Elf32_Ehdr hd);

/*
Entries: the section header
Result: print the section header
*/
void printSectionHeader(Elf32_Shdr_seq seqHd,Elf32_Ehdr hd,FILE *f);

/*
Entries: the section header, the index of the section, the elf header and the file
Result: the name of the section header
The program allocates memory for the string
*/
char * getSectionName(Elf32_Shdr_seq sh,int index, Elf32_Ehdr hd,FILE *f);

/******************************************************************************/
                            /* Section */
/******************************************************************************/

typedef struct char_array {
    unsigned char * tab;
    int n;
} char_array;

/*
Entries: f : the object file to read
arraySection : contains all the section of the file
sizeSection : the number sections
name : the name of the section to read
Result: a char_array which contains the values of the section
The program allocates memory for the array
*/
char_array readSectionByName(FILE* f, Elf32_Shdr_seq arraySection, char * name, Elf32_Ehdr hd);

/*
Entries: f : the object file to read
arraySection : contains all the section of the file
index : the index to the section to read
Result: a char_array which contains the values of the section
The program allocates memory for the array
*/
char_array readSectionByIndex(FILE* f, Elf32_Shdr_seq arraySection, int index);



/******************************************************************************/
                            /* Symbol Table */
/******************************************************************************/

typedef struct Elf32_Sym_seq {
    Elf32_Sym * tab;
    int n;
} Elf32_Sym_seq;

/*
Entries: f : the object file to read
arraySection : contains all the section of the file

Result: the table of symbol
The program allocates memory for the array
*/
Elf32_Sym_seq readSymbolTable(FILE* f, Elf32_Shdr_seq arraySection,Elf32_Ehdr hd) ;

/*
Entries: the symbol table, the section header, the file
Result: print the section header
*/
void printSymbolTable(Elf32_Sym_seq seqSym, Elf32_Shdr_seq arraySection,FILE *f);

/*
Entries: the symbol, the section header and the file
Result: the name of the symbol
The program allocates memory for the string
*/
char * getSymbolName(Elf32_Sym sym, Elf32_Shdr_seq seqSection, FILE *f);

#endif

/******************************************************************************/
                            /* Relocation Table */
/******************************************************************************/

typedef struct Elf32_Rel_expend {
    Elf32_Rel * tabRelocation;
    int n; //The number of element of the relocation table
    Elf32_Addr offset;
    char * name;
} Elf32_Rel_expend;

/*The array of relocation tables*/
typedef struct Elf32_Rel_seq {
    Elf32_Rel_expend * tab;
    int n; //Number of relocation table
} Elf32_Rel_seq;

/*
Entries: f : the object file to read
arraySection : contains all the section of the file

Result: the relocation table
The program allocates memory for the array
*/
Elf32_Rel_seq readRelocationTable(FILE* f, Elf32_Shdr_seq arraySection, Elf32_Ehdr hd);

/*
Entries: the relocation table, the section header, the file
Result: print the relocation table
*/
void printRelocationTable(Elf32_Rel_seq relT, Elf32_Shdr_seq arraySection, Elf32_Sym_seq symSeq,Elf32_Ehdr hd,FILE *f);
