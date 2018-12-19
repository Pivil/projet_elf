#include "relocation.h"


int main(int argc, char** argv){
	
	if(argc < 2){
		return 1;
	}
	FILE* file;
	Elf32_Ehdr hd;
	Elf32_Shdr_seq shd_o;
	file = fopen(argv[1],"r");
	secReorder(file,&shd_o);
	hd = readELFHeader(file);
	printf("B: %i\t A: %i\n",hd.e_shnum,shd_o.n);
	fclose(file);
	return 0;
}
