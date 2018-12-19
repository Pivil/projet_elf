#include "relocation.h"


void secReorder(FILE* input,Elf32_Shdr_seq* shd_o ){
	Elf32_Ehdr hd = readELFHeader(input);
	Elf32_Shdr_seq shd = readSectionHeader(input,hd);
	int i,j;
	j=0;
	shd_o->n = j;
	shd_o->tab = malloc(hd.e_shnum*sizeof(Elf32_Shdr));
	for(i = 0; i < hd.e_shnum; i++){

		if(shd.tab[i].sh_type != SHT_RELA && shd.tab[i].sh_type != SHT_REL){
			shd_o->tab[j] = shd.tab[i];
			j++;
		} 
		shd_o->n = j;
	}
	shd_o->tab = realloc(shd_o->tab,j*sizeof(Elf32_Shdr));
}
