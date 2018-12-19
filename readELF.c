#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readELF.h"
#include "byteManipulation.h"

/******************************************************************************/
                                /* ELF header */
/******************************************************************************/

Elf32_Ehdr readELFHeader(FILE *f) {
    Elf32_Ehdr hd;
	fseek(f,0,SEEK_SET);
    for(int i=0;i<16;i++)
        fscanf(f,"%c",&(hd.e_ident[i]));
    hd.e_type = get2Bytes(f);
    hd.e_machine = get2Bytes(f);
    hd.e_version = get4Bytes(f);
    hd.e_entry = get4Bytes(f); // This is the memory address of the entry point from where the process starts executing
    hd.e_phoff = get4Bytes(f); // Points to the start of the program header table
    hd.e_shoff = get4Bytes(f); // Points to the start of the section header table
    hd.e_flags = get4Bytes(f);
    hd.e_ehsize = get2Bytes(f); //Contains the size of this header, normally 64 Bytes for 64-bit and 52 Bytes for 32-bit format
    hd.e_phentsize = get2Bytes(f); // Contains the size of a program header table entry
    hd.e_phnum = get2Bytes(f); // Contains the number of entries in the program header table
    hd.e_shentsize = get2Bytes(f); // Contains the size of a section header table entry
    hd.e_shnum = get2Bytes(f); //Contains the number of entries in the section header table
    hd.e_shstrndx = get2Bytes(f); //Contains index of the section header table entry that contains the section names
    return hd;
}


void printELFHeader(Elf32_Ehdr hd) {
    printf("ELF header:\n");
    printf("Magic number: ");
    for(int i=0;i<16;i++)
        printf("%s%x ",hd.e_ident[i]<=9?"0":"",hd.e_ident[i]);
    printf("\n");
    printf("Class: %s\n", hd.e_ident[EI_CLASS]==1 ? "32" : "64");
    printf("Data: %s\n", hd.e_ident[EI_DATA]==1 ? "little endian" : "big endian");
    printf("Data: %d\n", hd.e_ident[EI_VERSION]);
    printf("OS/ABI: %x (%s) \n", hd.e_ident[EI_OSABI], hd.e_ident[EI_OSABI]==0x61 ? "ARM":"Other");
    printf("Type: ");
    switch (hd.e_type) {
        case 0:
            printf("NONE\n");
            break;
        case 1:
            printf("REL\n");
            break;
        case 2:
            printf("EXEC\n");
            break;
        default:
            printf("Other\n");
    }
    printf("Machine: ");
    switch (hd.e_machine) {
        case 0x28:
            printf("ARM\n");
            break;
        case 0x3e:
            printf("X86-64\n");
            break;
        default:
            printf("Other\n");
    }
    printf("Version: %d\n",hd.e_version);
    printf("Version: 0x%x\n",hd.e_entry);
    printf("Start of program headers: %d\n",hd.e_phoff);
    printf("Start of section headers: %d\n",hd.e_shoff);
    printf("Flags: 0x%x\n",hd.e_flags);
    printf("Size of this header: %d (bytes)\n",hd.e_ehsize);
    printf("Size of program headers: %d (bytes)\n",hd.e_phentsize);
    printf("Number of program headers: %d\n",hd.e_phnum);
    printf("Size of section headers: %d (bytes)\n",hd.e_shentsize);
    printf("Number of section headers: %d\n",hd.e_shnum);
    printf("Section header that contains section name: %d\n",hd.e_shstrndx);
}

/******************************************************************************/
                            /* Section header */
/******************************************************************************/


char * getSectionName(Elf32_Shdr_seq sh,int index, Elf32_Ehdr hd,FILE *f) {
    char *  str; int i=0;
    str = malloc(100);
    if (str==NULL) {
        exit(1);
    }
    str[0]=0;
    if (sh.tab[index].sh_name==0)
        return str;
    fseek(f,sh.tab[index].sh_name + sh.tab[hd.e_shstrndx].sh_offset,SEEK_SET);//Go the beginning of the string
    unsigned char c=1;
    while(c!='\0') {
        fscanf(f,"%c",&c);
        str[i]=c;
        i++;
    }
    return str;
}

Elf32_Shdr_seq readSectionHeader(FILE *f, Elf32_Ehdr hd) {
    Elf32_Shdr_seq seq;
    seq.n = hd.e_shnum;
    seq.tab = malloc(sizeof(Elf32_Shdr)*seq.n);

    fseek(f,hd.e_shoff,SEEK_SET); //Go to the beginning of the section header

    for(int i=0;i<seq.n;i++ ) {
        seq.tab[i].sh_name = get4Bytes(f);
        seq.tab[i].sh_type = get4Bytes(f);
        seq.tab[i].sh_flags = get4Bytes(f);
        seq.tab[i].sh_addr = get4Bytes(f);
        seq.tab[i].sh_offset = get4Bytes(f);
        seq.tab[i].sh_size = get4Bytes(f);
        seq.tab[i].sh_link = get4Bytes(f);
        seq.tab[i].sh_info = get4Bytes(f);
        seq.tab[i].sh_addralign = get4Bytes(f);
        seq.tab[i].sh_entsize = get4Bytes(f);
    }
    return seq;
}


void printSectionHeader(Elf32_Shdr_seq seqHd,Elf32_Ehdr hd,FILE *f) {
    char * str;
    printf("[Nr] Name                 Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
    for(int i=0;i<seqHd.n;i++) {
        printf("[%2d] ",i);
        str = getSectionName(seqHd,i,hd,f);
        printf("%s ",str);
        for(int k = strlen(str);k<20 ;k++) { //For the alginement of column
            printf(" ");
        }

        char *strType;
        switch (seqHd.tab[i].sh_type) {
            case SHT_NULL:
            strType="NULL";
            break;
            case SHT_PROGBITS:
            strType="PROGBITS";
            break;
            case SHT_STRTAB:
            strType="STRTAB";
            break;
            case SHT_SYMTAB:
            strType="SYMTAB";
            break;
            case SHT_RELA:
            strType="RELA";
            break;
            case SHT_HASH :
            strType="HASH";
            break;
            case SHT_DYNAMIC:
            strType="DYNAMIC";
            break;
            case SHT_NOTE :
            strType="NOTE";
            break;
            case SHT_NOBITS :
            strType="NOBITS";
            break;
            case SHT_REL :
            strType="REL";
            break;
            case SHT_SHLIB :
            strType="SHLIB";
            break;
            case SHT_DYNSYM :
            strType="DYNSYM";
            break;
            case SHT_NUM :
            strType="NUM";
            break;
            case SHT_LOPROC :
            strType="LOPROC";
            break;
            case SHT_HIPROC :
            strType="HIPROC";
            break;
            case SHT_LOUSER :
            strType="LOUSER";
            break;
            case SHT_HIUSER :
            strType="HIUSER";
            break;
            case SHT_ARM_ATTRIBUTES :
            strType="ARM_ATTRIBUTES";
            break;
            default:
                strType="UNKNOWN";
        }
        printf("%s",strType);
        for(int k = strlen(strType);k<16 ;k++) { //For the alginement of column
            printf(" ");
        }

        printf("%8.8x ",seqHd.tab[i].sh_addr);
        printf("%6.6x ",seqHd.tab[i].sh_offset);
        printf("%6.6x ",seqHd.tab[i].sh_size);
        printf("%2.2x ",seqHd.tab[i].sh_entsize);

        int a = 0; char strFlag[5]; unsigned int testFlag=seqHd.tab[i].sh_flags;
        if(testFlag>=SHF_MASKPROC) {
            testFlag -= SHF_MASKPROC;
            strFlag[0]='M';
            a++;
        }
        if(testFlag>=SHF_EXECINSTR) {
            testFlag -= SHF_EXECINSTR;
            strFlag[a]='X';
            a++;
        }
        if(testFlag>=SHF_ALLOC) {
            testFlag -= SHF_ALLOC;
            strFlag[a]='A';
            a++;
        }
        if(testFlag>=SHF_WRITE) {
            testFlag -= SHF_WRITE;
            strFlag[a]='W';
            a++;
        }
        strFlag[a] = '\0';
        printf("%s",strFlag); //Write the flags
        for(int k = strlen(strFlag);k<4 ;k++) { //For the alginement of column
            printf(" ");
        }

        printf("%2d ",seqHd.tab[i].sh_link);
        printf("%3d  ",seqHd.tab[i].sh_info);
        printf("%d\n",seqHd.tab[i].sh_addralign);
        free(str);
    }
}


/******************************************************************************/
                            /* Section */
/******************************************************************************/


char_array readSectionByName(FILE* f, Elf32_Shdr_seq arraySection, char * name, Elf32_Ehdr hd) {
    char_array h;
    int found=-1;
    /*Test the string*/
    for(int i=0;i<arraySection.n;i++) {
        if(!strcmp(getSectionName(arraySection,i,hd,f),name)) {
            found = i;
        }
    }
    if (found==-1) { //The section is not present
        printf("ERROR: name not found in the section header!\n");
        exit(1);
    }
    else {
        h.n = arraySection.tab[found].sh_size;
        h.tab=malloc(h.n); //Alloc the array

        fseek(f,arraySection.tab[found].sh_offset,SEEK_SET); //Go to the beginning of the section
        for(unsigned int i=0;i<arraySection.tab[found].sh_size;i++) { //Read the section
            fscanf(f,"%c",&(h.tab[i]));
        }
        return h;
    }
}

char_array readSectionByIndex(FILE* f, Elf32_Shdr_seq arraySection, int index) {
    char_array h;
    if (index > arraySection.n) { // error, the index is not in the section
        exit(1);
    }
    h.n = arraySection.tab[index].sh_size;
    h.tab=malloc(h.n); //Alloc the array

    fseek(f,arraySection.tab[index].sh_offset,SEEK_SET); //Go to the beginning of the section
    for(unsigned int i=0;i<arraySection.tab[index].sh_size;i++) { //Read the section
        fscanf(f,"%c",&(h.tab[i]));
    }
    return h;
}

/******************************************************************************/
                            /* Symbol Table */
/******************************************************************************/

char * getSymbolName(Elf32_Sym sym, Elf32_Shdr_seq seqSection, FILE *f) {
    char *str = malloc(100);
    /*for(int i=0;indexNameSymbol==-1 && i<seqSection.n;i++) { //Find the index of the symbol table in the header section
        if(seqSection.tab[i].sh_type == SHT_SYMTAB)
            indexNameSymbol=1;
    }*/
    if (sym.st_name==0)
        return str;

    //TODO On récupère directement la valeur (il vaut mieux parcourir)
    fseek(f,sym.st_name + seqSection.tab[seqSection.n-1].sh_offset,SEEK_SET);//Go the beginning of the string
    unsigned char c=1; int i=0;
    while(c!='\0') {
        fscanf(f,"%c",&c);
        str[i]=c;
        i++;
    }

    return str;
}

Elf32_Sym_seq readSymbolTable(FILE* f, Elf32_Shdr_seq arraySection) {
    //Size of a symbol 16bytes
    int index=-1;
    for(int i=0;index==-1 && i<arraySection.n;i++) { //Find the index of the symbol table in the header section
        if(arraySection.tab[i].sh_type==SHT_SYMTAB)
            index=i;
    }

    fseek(f,arraySection.tab[index].sh_offset,SEEK_SET); //Got the beginning of the symbol table

    int nbSymbol = arraySection.tab[index].sh_size/16;
    Elf32_Sym_seq seqSym;
    seqSym.n = nbSymbol;
    seqSym.tab = malloc(nbSymbol*(sizeof(Elf32_Sym)));

    int iSym=0;
    for(int i=0;i<nbSymbol;i++) { //For each symbol
        seqSym.tab[iSym].st_name = get4Bytes(f);
        seqSym.tab[iSym].st_value = get4Bytes(f);
        seqSym.tab[iSym].st_size = get4Bytes(f);
        fscanf(f,"%c",&(seqSym.tab[iSym].st_info));
        fscanf(f,"%c",&(seqSym.tab[iSym].st_other));
        seqSym.tab[iSym].st_shndx = get2Bytes(f);
        iSym++;
    }
    return seqSym;
}

void printSymbolTable(Elf32_Sym_seq seqSym, Elf32_Shdr_seq arraySection,FILE *f) {
    printf("Num:    Value  Size Type    Bind   Vis      Ndx Name\n");
    for(int iSym=0;iSym<seqSym.n;iSym++) {
        printf("%3d: ", iSym);
        printf("%8.8x ", seqSym.tab[iSym].st_value);
        printf("%4d ", seqSym.tab[iSym].st_size);

        switch (ELF32_ST_TYPE(seqSym.tab[iSym].st_info)) {
            case STT_NOTYPE:
            printf("%7.7s ","NOTYPE");
            break;
            case STT_OBJECT:
            printf("%7.7s ","OBJECT");
            break;
            case STT_FUNC:
            printf("%7.7s ","FUNC");
            break;
            case STT_SECTION:
            printf("%7.7s ","SECTION");
            break;
            case STT_FILE:
            printf("%7.7s ","WEFILEAK");
            break;
            case STT_COMMON:
            printf("%7.7s ","COMMON");
            break;
            case STT_TLS:
            printf("%7.7s ","TLS");
            break;
            default:
            printf("%7.7s ","UNKNOWN");
            break;
        }

        switch (ELF32_ST_BIND(seqSym.tab[iSym].st_info)) {
            case STB_LOCAL:
            printf("%6.6s ","LOCAL");
            break;
            case STB_GLOBAL:
            printf("%6.6s ","LOCAL");
            break;
            case STB_WEAK:
            printf("%6.6s ","WEAK");
            break;
            default:
            printf("%6.6s ","UNKNOWN");
            break;
        }
        if(iSym==0)
            printf("UND     ");
        else
            printf("DEFAULT ");
        printf("%3d   ",seqSym.tab[iSym].st_shndx);
        printf("%4.15s ",getSymbolName(seqSym.tab[iSym],arraySection,f));
        printf("\n");
    }
}

/******************************************************************************/
                            /* Relocation Table */
/******************************************************************************/

Elf32_Rel_seq readRelocationTable(FILE* f, Elf32_Shdr_seq arraySection, Elf32_Ehdr hd) {
    Elf32_Rel_seq seqRel; int nbRelocation=0;

    for (int i=0;i<arraySection.n;i++) { //Count the number of relocation
        if(arraySection.tab[i].sh_type == SHT_REL) { //A relocation table
            nbRelocation++;
        }
    }
    seqRel.n=nbRelocation;
    seqRel.tab = malloc(sizeof(Elf32_Rel_expend)*nbRelocation);
    int iTableReloc=0;
    for (int i=0;i<arraySection.n;i++) {
        if(arraySection.tab[i].sh_type == SHT_REL) { //A relocation table
            seqRel.tab[iTableReloc].n = arraySection.tab[i].sh_size/8;
            seqRel.tab[iTableReloc].tabRelocation = malloc(arraySection.tab[i].sh_size);
            seqRel.tab[iTableReloc].offset = arraySection.tab[i].sh_offset;
            seqRel.tab[iTableReloc].name = getSectionName(arraySection,i,hd,f);

            fseek(f,arraySection.tab[i].sh_offset,SEEK_SET); //Go to the section for the relocation

            for(int k=0;k<seqRel.tab[iTableReloc].n;k++) { //For each value of the relocation table
                seqRel.tab[iTableReloc].tabRelocation[k].r_offset = get4Bytes(f);
                seqRel.tab[iTableReloc].tabRelocation[k].r_info = get4Bytes(f);
            }
            iTableReloc ++;
        }
    }
    return seqRel;
}

void printRelocationTable(Elf32_Rel_seq relT, Elf32_Shdr_seq arraySection, Elf32_Sym_seq symSeq,Elf32_Ehdr hd,FILE *f) {

    for(int i=0;i<relT.n;i++) {// For each table of relocation
        printf("Relocation section %s at offset 0x%x: \n",relT.tab[i].name,relT.tab[i].offset);
        printf("  Offset     Info    Type       NumSymbol Sym.Value Sym.Name     Section.Name\n");

        for(int k=0;k<relT.tab[i].n;k++) {// For each value of the table
            printf("%8.8x ",relT.tab[i].tabRelocation[k].r_offset);
            printf("%8.8x ",relT.tab[i].tabRelocation[k].r_info);

            switch (ELF32_R_TYPE(relT.tab[i].tabRelocation[k].r_info)) {
                case R_ARM_NONE:
                printf("%12.12s ","R_ARM_NONE");
                break;
                case R_ARM_ABS32:
                printf("%12.12s ","R_ARM_ABS32");
                break;
                case R_ARM_ABS16:
                printf("%12.12s","R_ARM_ABS16");
                break;
                case R_ARM_ABS8:
                printf("%12.12s ","R_ARM_ABS8");
                break;
                case R_ARM_CALL:
                printf("%12.12s ","R_ARM_CALL");
                break;
                case R_ARM_JUMP24:
                printf("%12.12s ","R_ARM_JUMP24");
                break;
                default:
                printf("%12.12s ","UNKNWON");
                break;
            }
            unsigned int numSym = ELF32_R_SYM(relT.tab[i].tabRelocation[k].r_info);
            printf(" %8.8x  ",numSym);//The num of the symbol
            printf("%8.8x",symSeq.tab[numSym].st_value); //The symbol Value
            printf("%16.16s",getSymbolName(symSeq.tab[numSym],arraySection ,f)); //The symbol name
            printf("%s",getSectionName(arraySection,symSeq.tab[numSym].st_shndx,hd,f)); //The section concerned name
            printf("\n");
        }
        printf("\n");
    }
}
