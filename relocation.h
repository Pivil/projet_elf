#ifndef __RELOCATION_H__	
#define __RELOCATION_H__
#include "readELF.h"
#include <stdlib.h>

void secReorder(FILE* input,Elf32_Shdr_seq* shd_o);

#endif
