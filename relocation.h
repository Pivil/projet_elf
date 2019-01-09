#ifndef __RELOCATION_H__	
#define __RELOCATION_H__
#include "readELF.h"
#include <stdlib.h>
#include "byteManipulation.h"

void secReorder(FILE* input,Elf32_Shdr_seq* shd_o, Elf32_Ehdr* hd_o,int* oldIds);

void writeEHDR(Elf32_Ehdr* hd, FILE* output);
void writeSHDRT(Elf32_Shdr* shdt,int shdtSize, FILE* output, uint32_t offset);
void copySection(FILE* input, FILE* output, uint32_t in_offset, uint32_t out_offset, uint32_t size);
void resizeFile(FILE* file, uint32_t offset, uint32_t size);
int getAddress(char* address);
void symbolImplantation(FILE* result, Elf32_Ehdr* ehdr, Elf32_Shdr_seq* arraySection, int* oldIds, int addData, int addText);
void writeSYMB (Elf32_Shdr_seq tabShdr, Elf32_Sym_seq* symboles, FILE* file);

#endif
