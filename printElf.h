#ifndef __PRINT_ELF_H__
#define __PRINT_ELF_H__
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>

typedef Elf32_Shdr* SHdrTable ;

void shtToStr(uint32_t sht, char* str);

void etToStr(uint16_t e_type, char* str);

void emToStr(uint16_t e_machine,char* str);

void print_ehdr(FILE* file, Elf32_Ehdr* ehdr);

void print_shdr(FILE* file, SHdrTable* shdr, uint32_t offset, uint16_t hdrSize, uint16_t hdrNb);

void print_sym(FILE* file, SHdrTable shdr, uint16_t shdrSize);


#endif

