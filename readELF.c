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
    for(int i=0;i<16;i++) {
        hd.e_ident[i]=get1Byte(f);
    }

    char endianness = hd.e_ident[EI_DATA];
    hd.e_type = get2Bytes(f,endianness);
    hd.e_machine = get2Bytes(f,endianness);
    hd.e_version = get4Bytes(f,endianness);
    hd.e_entry = get4Bytes(f,endianness); // This is the memory address of the entry point from where the process starts executing
    hd.e_phoff = get4Bytes(f,endianness); // Points to the start of the program header table
    hd.e_shoff = get4Bytes(f,endianness); // Points to the start of the section header table
    hd.e_flags = get4Bytes(f,endianness);
    hd.e_ehsize = get2Bytes(f,endianness); //Contains the size of this header, normally 64 Bytes for 64-bit and 52 Bytes for 32-bit format
    hd.e_phentsize = get2Bytes(f,endianness); // Contains the size of a program header table entry
    hd.e_phnum = get2Bytes(f,endianness); // Contains the number of entries in the program header table
    hd.e_shentsize = get2Bytes(f,endianness); // Contains the size of a section header table entry
    hd.e_shnum = get2Bytes(f,endianness); //Contains the number of entries in the section header table
    hd.e_shstrndx = get2Bytes(f,endianness); //Contains index of the section header table entry that contains the section names
	return hd;
}


void printELFHeader(Elf32_Ehdr hd) {
    printf("ELF header:\n");
    printf("Magic number: ");
    for(int i=0;i<16;i++)
        printf("%s%x ",hd.e_ident[i]<=9?"0":"",hd.e_ident[i]);
    printf("\n");
    printf("Class: ELF%s\n", hd.e_ident[EI_CLASS]==1 ? "32" : "64");
    printf("Data: %s\n", hd.e_ident[EI_DATA]==1 ? "little endian" : "big endian");
    printf("Version: %d\n", hd.e_ident[EI_VERSION]);
    printf("OS/ABI: ");
    switch (hd.e_ident[EI_OSABI]) {
        case 0x61:
            printf("ARM\n");
            break;
        case 0x0:
            printf("UNIX - System V\n");
            break;
        default:
            printf("Other\n");
    }
    printf("Abi version: %d\n", hd.e_ident[EI_PAD]);
    printf("Type: ");
    switch (hd.e_type) {
        case 0:
            printf("NONE\n");
            break;
        case 1:
            printf("REL (Relocatable file)\n");
            break;
        case 2:
            printf("EXEC (Executable file)\n");
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
            printf("Advanced Micro Devices X86-64\n");
            break;
        default:
            printf("Other\n");
    }
    printf("Version: 0x%x\n",hd.e_version);
    printf("Entry point adress: 0x%x\n",hd.e_entry);
    printf("Start of program headers: %d (bytes into file)\n",hd.e_phoff);
    printf("Start of section headers: %d (bytes into file)\n",hd.e_shoff);
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
    char *  str;
    str = malloc(100);
    if (str==NULL) {
        exit(1);
    }
    str[0]=0;
    if (sh.tab[index].sh_name==0)
        return str;
    fseek(f,sh.tab[index].sh_name + sh.tab[hd.e_shstrndx].sh_offset,SEEK_SET);//Go the beginning of the string
    unsigned char c=1; int i=0;
    while(c!='\0') {
        c=get1Byte(f);
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
    char endianness = hd.e_ident[EI_DATA];
    for(int i=0;i<seq.n;i++ ) {
        seq.tab[i].sh_name = get4Bytes(f,endianness);
        seq.tab[i].sh_type = get4Bytes(f,endianness);
        seq.tab[i].sh_flags = get4Bytes(f,endianness);
        seq.tab[i].sh_addr = get4Bytes(f,endianness);
        seq.tab[i].sh_offset = get4Bytes(f,endianness);
        seq.tab[i].sh_size = get4Bytes(f,endianness);
        seq.tab[i].sh_link = get4Bytes(f,endianness);
        seq.tab[i].sh_info = get4Bytes(f,endianness);
        seq.tab[i].sh_addralign = get4Bytes(f,endianness);
        seq.tab[i].sh_entsize = get4Bytes(f,endianness);
    }
    return seq;
}


void printSectionHeader(Elf32_Shdr_seq seqHd,Elf32_Ehdr hd,FILE *f) {
    char * str;
    printf("[Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
    for(int i=0;i<seqHd.n;i++) {
        printf("[%2d] ",i);
        str = getSectionName(seqHd,i,hd,f);
        printf("%-17.17s ",str);

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
        printf("%-16.16s",strType);

        printf("%8.8x ",seqHd.tab[i].sh_addr);
        printf("%6.6x ",seqHd.tab[i].sh_offset);
        printf("%6.6x ",seqHd.tab[i].sh_size);
        printf("%2.2x ",seqHd.tab[i].sh_entsize);

        int a = 0; char strFlag[5]; unsigned int testFlag=seqHd.tab[i].sh_flags;

        if((testFlag&SHF_MASKPROC)==SHF_MASKPROC) {
            strFlag[0]='M';
            a++;
        }
        if((testFlag&SHF_WRITE)==SHF_WRITE) {
            strFlag[a]='W';
            a++;
        }
        if((testFlag&SHF_ALLOC)==SHF_ALLOC) {
            strFlag[a]='A';
            a++;
        }
        if((testFlag&SHF_EXECINSTR)==SHF_EXECINSTR) {
            strFlag[a]='X';
            a++;
        }

        strFlag[a] = '\0';
        printf("%3.3s ",strFlag); //Write the flags

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
    char *nameSection=NULL;
    /*Test the string*/
    for(int i=0;i<arraySection.n;i++) {
        nameSection = getSectionName(arraySection,i,hd,f);
        if(!strcmp(nameSection,name)) {
            found = i;
        }
        free(nameSection);
    }
    if (found==-1) { //The section is not present
        printf("ERROR: name not found in the section header!\n");
        exit(1);
    }
    else {
        h.n = arraySection.tab[found].sh_size;
        h.tab=malloc(h.n); //Alloc the array
        h.offset = arraySection.tab[found].sh_addr; //The beginning of the section

        fseek(f,arraySection.tab[found].sh_offset,SEEK_SET); //Go to the beginning of the section
        for(unsigned int i=0;i<arraySection.tab[found].sh_size;i++) { //Read the section
            h.tab[i] = get1Byte(f);
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
    h.offset = arraySection.tab[index].sh_addr; //The beginning of the section
    fseek(f,arraySection.tab[index].sh_offset,SEEK_SET); //Go to the beginning of the section
    for(unsigned int i=0;i<arraySection.tab[index].sh_size;i++) { //Read the section
        h.tab[i] = get1Byte(f);
    }
    return h;
}

void printSection(char_array h) {
    if (h.n==0) {
        printf("The section is empty\n");
    }
    for(int i=0;i<h.n;i++) {
        if (i%16==0) { //Print the adress
            printf("\n0x%8.8x ",i+h.offset);
        }

        printf("%2.2x",h.tab[i]); //Print the value
        if ((i+1)%4==0) {
            printf(" ");
        }
        if((i+1)%16==0) { //Print a string value
            for(int k=i-15;k<=i;k++) {
                if (h.tab[k]>=32 && h.tab[k]<=126) {
                    printf("%c",h.tab[k]);
                }
                else {
                    printf(".");
                }
            }
        }
        else if (i==h.n-1) {  // Last line: Print a string value
            for(int k=0;k<=15-(i%16);k++) { //Print the white space
                if((k+((i+1)%4)%4==0))
                    printf(" ");
                printf("  ");
            }
            for(int k=i-(h.n%16)+1;k<=i;k++) {
                if (h.tab[k]>=32 && h.tab[k]<=126) {
                    printf("%c",h.tab[k]);
                }
                else {
                    printf(".");
                }
            }
        }
    }
    printf("\n");
}

/******************************************************************************/
                            /* Symbol Table */
/******************************************************************************/

char * getSymbolName(Elf32_Sym sym, Elf32_Shdr_seq seqSection, FILE *f) {
    char *str = malloc(100);
    str[0]=0; //Unintialize to an empty string
    if (sym.st_name==0)
        return str;

    //TODO On récupère directement la valeur (il vaut mieux parcourir)
    fseek(f,sym.st_name + seqSection.tab[seqSection.n-1].sh_offset,SEEK_SET);//Go the beginning of the string
    int i=0; unsigned char c=1;
    while(c!='\0') { //While we're not at the end of the string
        c=get1Byte(f); //Read the file
        str[i]=c;
        i++;
    }

    return str;
}

int getSymbolValue(Elf32_Sym sym, Elf32_Shdr_seq seqSection,Elf32_Ehdr hd, FILE *f) {
    int sectionAdr = seqSection.tab[sym.st_shndx].sh_offset; //Section address
    int symbolAdr = sym.st_value + sectionAdr; //The address of the value of the symbol
    fseek(f,symbolAdr,SEEK_SET);//Go the beginning of the symbol value
    char endianness = hd.e_ident[EI_DATA];
    return get4Bytes(f,endianness); //Get the value in the file
}

Elf32_Sym_seq readSymbolTable(FILE* f, Elf32_Shdr_seq arraySection,Elf32_Ehdr hd) {
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
    char endianness = hd.e_ident[EI_DATA];
    for(int i=0;i<nbSymbol;i++) { //For each symbol
        seqSym.tab[iSym].st_name = get4Bytes(f,endianness);
        seqSym.tab[iSym].st_value = get4Bytes(f,endianness);
        seqSym.tab[iSym].st_size = get4Bytes(f,endianness);
        seqSym.tab[iSym].st_info = get1Byte(f);
        seqSym.tab[iSym].st_other = get1Byte(f);
        seqSym.tab[iSym].st_shndx = get2Bytes(f,endianness);
        iSym++;
    }
    return seqSym;
}

void printSymbolTable(Elf32_Sym_seq seqSym, Elf32_Shdr_seq arraySection,FILE *f) {
    printf("Num:    Value  Size Type    Bind   Vis      Ndx Name\n");
    printf("  0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND \n");
    char * nameSymbol=NULL;
    for(int iSym=1;iSym<seqSym.n;iSym++) {
        printf("%3d: ", iSym);
        printf("%8.8x  ", seqSym.tab[iSym].st_value);
        printf("%4d ", seqSym.tab[iSym].st_size);

        switch (ELF32_ST_TYPE(seqSym.tab[iSym].st_info)) {
            case STT_NOTYPE:
            printf("NOTYPE  ");
            break;
            case STT_OBJECT:
            printf("OBJECT  ");
            break;
            case STT_FUNC:
            printf("FUNC    ");
            break;
            case STT_SECTION:
            printf("SECTION ");
            break;
            case STT_FILE:
            printf("FILE    ");
            break;
            case STT_COMMON:
            printf("COMMON  ");
            break;
            case STT_TLS:
            printf("TLS     ");
            break;
            default:
            printf("UNKNOWN");
            break;
        }

        switch (ELF32_ST_BIND(seqSym.tab[iSym].st_info)) {
            case STB_LOCAL:
            printf("LOCAL  ");
            break;
            case STB_GLOBAL:
            printf("GLOBAL ");
            break;
            case STB_WEAK:
            printf("WEAK    ");
            break;
            default:
            printf("UNKNOWN");
            break;
        }
        printf("DEFAULT  ");

        if(seqSym.tab[iSym].st_shndx==SHN_ABS)
            printf("ABS ");
        else if(seqSym.tab[iSym].st_shndx==SHN_UNDEF)
            printf("UND ");
        else
            printf("%3d ",seqSym.tab[iSym].st_shndx);
        nameSymbol = getSymbolName(seqSym.tab[iSym],arraySection,f);
        printf("%s ",nameSymbol);
        free(nameSymbol);
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
    seqRel.n=nbRelocation; //The number of relocation section
    seqRel.tab = malloc(sizeof(Elf32_Rel_expend)*nbRelocation);
    int iTableReloc=0;
    for (int i=0;i<arraySection.n;i++) { //For each section
        if(arraySection.tab[i].sh_type == SHT_REL) { //A relocation table
            seqRel.tab[iTableReloc].n = arraySection.tab[i].sh_size/8; //The number of entries for this relocation section
            seqRel.tab[iTableReloc].tabRelocation = malloc(arraySection.tab[i].sh_size);
            seqRel.tab[iTableReloc].offset = arraySection.tab[i].sh_offset; //Offset of the reloc section
            seqRel.tab[iTableReloc].name = getSectionName(arraySection,i,hd,f); //Name of the reloc section

            fseek(f,arraySection.tab[i].sh_offset,SEEK_SET); //Go to the section for the relocation
            char endianness = hd.e_ident[EI_DATA];
            for(int k=0;k<seqRel.tab[iTableReloc].n;k++) { //For each value of the relocation table
                seqRel.tab[iTableReloc].tabRelocation[k].r_offset = get4Bytes(f,endianness);
                seqRel.tab[iTableReloc].tabRelocation[k].r_info = get4Bytes(f,endianness);
            }
            iTableReloc ++;
        }
    }
    return seqRel;
}

void printRelocationTable(Elf32_Rel_seq relT, Elf32_Shdr_seq arraySection, Elf32_Sym_seq symSeq,Elf32_Ehdr hd,FILE *f) {

    for(int i=0;i<relT.n;i++) {// For each table of relocation
        printf("\n");
        printf("Relocation section '%s' at offset 0x%x contains %d entries:\n",relT.tab[i].name,relT.tab[i].offset,relT.tab[i].n);
        printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");

        for(int k=0;k<relT.tab[i].n;k++) {// For each value of the table
            printf("%8.8x  ",relT.tab[i].tabRelocation[k].r_offset); //Print offset
            printf("%8.8x ",relT.tab[i].tabRelocation[k].r_info); //Print info

            switch (ELF32_R_TYPE(relT.tab[i].tabRelocation[k].r_info)) { //Print the type of relocation
                case R_ARM_NONE:
                printf("%-17.17s ","R_ARM_NONE");
                break;
                case R_ARM_ABS32:
                printf("%-17.17s ","R_ARM_ABS32");
                break;
                case R_ARM_ABS16:
                printf("%-17.17s","R_ARM_ABS16");
                break;
                case R_ARM_ABS8:
                printf("%-17.17s ","R_ARM_ABS8");
                break;
                case R_ARM_CALL:
                printf("%-17.17s ","R_ARM_CALL");
                break;
                case R_ARM_PC24:
                printf("%-17.17s ","R_ARM_PC24");
                break;
                case R_ARM_JUMP24:
                printf("%-17.17s ","R_ARM_JUMP24");
                break;
                default:
                printf("%-17.17s ","UNKNWON");
                break;
            }
            unsigned int numSym = ELF32_R_SYM(relT.tab[i].tabRelocation[k].r_info); //Get the concerned index of the symbol table
            Elf32_Sym concernedSym = symSeq.tab[numSym]; //The concerned relocation symbol
            printf("%8.8x   ",concernedSym.st_value); //The symbol Value
            if((ELF32_ST_TYPE(concernedSym.st_info)==STT_NOTYPE || ELF32_ST_TYPE(concernedSym.st_info)==STT_FUNC)) {
                printf("%s",getSymbolName(symSeq.tab[numSym],arraySection,f));//The symbol concerned name
            }
            else {
                printf("%s",getSectionName(arraySection,symSeq.tab[numSym].st_shndx,hd,f)); //The section concerned name
            }
            printf("\n");
        }
    }
}
