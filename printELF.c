#include "readELF.h"
#include "elf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printHelp() {
    printf("Options: \n\
    Print the ELF header: -h nameFile \n\
    Print the Section header: -S nameFile \n\
    Print a section by name : -x nameSection nameFile \n\
    Print a section by index : -x indexSection nameFile \n\
    Print the symbol table : -s nameFile \n\
    Print the relocation tables : -r nameFile \n\
    ");
}

int isNumber(char * str) {
    int i=0; int isN=1;
    while(isN && str[i] != '\0') {
        if(str[i]<'0' || str[i]>'9')
            isN=0;
        i++;
    }
    return isN;
}

int main(int argc,char * argv[]) {

    if (argc==3) {
        if(!strcmp(argv[1],"-h") || !strcmp(argv[1],"--headerELF")) { //Print the ELF header
            FILE * f = fopen(argv[2],"r");
            Elf32_Ehdr hd = readELFHeader(f);
            printELFHeader(hd);
            fclose(f);
        }

        else if(!strcmp(argv[1],"-S") || !strcmp(argv[1],"--headerSection")) { //Print the Section header
            FILE * f = fopen(argv[2],"r");
            Elf32_Ehdr hd = readELFHeader(f);
            Elf32_Shdr_seq shd = readSectionHeader(f,hd);
            printSectionHeader(shd,hd,f);
            free(shd.tab);
            fclose(f);
        }

        else if(!strcmp(argv[1],"-s") || !strcmp(argv[1],"--symbolTable")) { //Print the symbol table
            FILE * f = fopen(argv[2],"r");
            Elf32_Ehdr hd = readELFHeader(f);
            Elf32_Shdr_seq shd = readSectionHeader(f,hd);
            Elf32_Sym_seq symTab = readSymbolTable(f,shd);
            printSymbolTable(symTab,shd,f);
            free(shd.tab); free(symTab.tab);
            fclose(f);
        }

        /*TODO finish*/
        else if(!strcmp(argv[1],"-r") || !strcmp(argv[1],"--relocationTable")) { //Print the relocation table
            FILE * f = fopen(argv[2],"r");
            Elf32_Ehdr hd = readELFHeader(f);
            Elf32_Shdr_seq shd = readSectionHeader(f,hd);
            Elf32_Sym_seq symTab = readSymbolTable(f,shd);
            Elf32_Rel_seq seqRel = readRelocationTable(f,shd,symTab,hd);

            printRelocationTable(seqRel,shd,f);
            free(shd.tab);free(symTab.tab); //TODO free
            fclose(f);
        }

    }
    else if (argc==4) {
        char_array h;
        if(!strcmp(argv[1],"-x") || !strcmp(argv[1],"--section")) {
            if(isNumber(argv[2])) { //Select the section by index
                FILE* f = fopen(argv[3],"r");
                Elf32_Ehdr hd = readELFHeader(f);
                Elf32_Shdr_seq shd = readSectionHeader(f,hd);

                h = readSectionByIndex(f,shd, atoi(argv[2]));
                if (h.n==0) {
                    printf("The section is empty\n");
                }
                for(int i=0;i<h.n;i++) {
                    if (i%16==0) { //Print the adress
                        printf("\n0x%8.8x ",i);
                    }
                    printf("%2.2x",h.tab[i]);
                    if ((i+1)%4==0) {
                        printf(" ");
                    }
                }
                printf("\n");
                free(shd.tab);free(h.tab);
                fclose(f);
            }
            else { //Select the section by name
                FILE* f = fopen(argv[3],"r");
                Elf32_Ehdr hd = readELFHeader(f);
                Elf32_Shdr_seq shd = readSectionHeader(f,hd);

                h = readSectionByName(f, shd, argv[2],hd);
                if (h.n==0) {
                    printf("The section is empty!\n");
                }
                for(int i=0;i<h.n;i++) {
                    if (i%16==0) { //Print the adress
                        printf("\n0x%8.8x ",i);
                    }
                    printf("%2.2x",h.tab[i]); //Print the value
                    if ((i+1)%4==0) {
                        printf(" ");
                    }
                }
                printf("\n");
                free(shd.tab); free(h.tab);
                fclose(f);
            }
        }
        else { //Bad values
            printHelp();
        }
    }
    else { //Bad values
        printHelp();
    }

    return 1;


    return 0;
}
