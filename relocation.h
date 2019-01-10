#ifndef __RELOCATION_H__	
#define __RELOCATION_H__
#include "readELF.h"
#include <stdlib.h>
#include "byteManipulation.h"

/*
Entrie: 
input 	: input file
shd_o 	: section header array
hd_o 	: ELF header
oldIds	: array of int used to update section ids

result 	: delete unused section and reorder used section 
*/
void secReorder(FILE* input, Elf32_Shdr_seq* shd_o, Elf32_Ehdr* hd_o, int* oldIds);

/*
Entrie: 
hd 		: ELF header
output 	: output file
Result: write the header data in the output file
*/
void writeEHDR(Elf32_Ehdr* hd, FILE* output);

/*
Entrie: 
shdt		: section header
shdtSize 	: number of section to write
output 		: output file
offset 		: offset of section header
Result: write the section header data in the output file, using the offset
*/
void writeSHDRT(Elf32_Shdr* shdt,int shdtSize, FILE* output, uint32_t offset);

/*
Entrie: 
input 		: input file
output		: output file
in_offset	: input file section offset
out_offset	: output file section offset
size 		: size of the sections
Result: copy the section data from input file to output file
*/
void copySection(FILE* input, FILE* output, uint32_t in_offset, uint32_t out_offset, uint32_t size);

/*
Entrie: 
file 		: file to resize
offset 	/ size 	: use the offset + the size to resize file

Result: the file is resized of (offset + size) bytes 
*/
void resizeFile(FILE* file, uint32_t offset, uint32_t size);

/*
Entrie: a String to get the address 
Result: the address in decimal
*/
int getAddress(char* address);

/*
Entrie: 
file         		: the output file
ehdr         		: the header of ELF file
arraySection 		: the section header array
oldIds   	 		:  an array of int to update section ids
addData / addText 	: the address of .data / .text

Result: update the section ids and set the symbole value with absolue value, then use writeSYMB to write new values in file

*/
void updateSymbols(FILE* result, Elf32_Ehdr* ehdr, Elf32_Shdr_seq* arraySection, int* oldIds, int addData, int addText);

/*
Entrie: 
tabShdr 	: section header array
symboles 	: symbols array, updated by function updateSymbols
file 		: output file

Result 		: write new values in file
*/
void writeSYMB (Elf32_Shdr_seq tabShdr, Elf32_Sym_seq* symboles, FILE* file);

#endif
