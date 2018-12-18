#include "printElf.h"
int main(int argc, char** argv){
	Elf32_Ehdr ehdr;
	SHdrTable shdr;
	FILE* file;

	shdr = NULL;

	if(argc < 2){
		return 0;
	}	

	file = fopen(argv[1],"rb");
	print_ehdr(file, &ehdr);
	print_shdr(file, &shdr, ehdr.e_shoff, ehdr.e_shentsize, ehdr.e_shnum);

	printf(" --- Debut test ---\n");

	print_sym(file, shdr, ehdr.e_shnum);
	
	
	printf(" --- Fin test ---\n");
	free(shdr);
	fclose(file);
	return 0;
}

