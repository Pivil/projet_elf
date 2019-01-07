#include "relocation.h"


int main(int argc, char** argv){

	if(argc < 3){
		return 1;
	}
	FILE* file,*result;
	Elf32_Ehdr hd_o;
	Elf32_Ehdr old_hd;
	Elf32_Shdr_seq old_shd;
	Elf32_Shdr_seq shd_o;
	file = fopen(argv[1],"r");
	result = fopen(argv[2], "w");
	old_hd = readELFHeader(file);
	old_shd = readSectionHeader(file,old_hd);
	int* oldIds = malloc(old_hd.e_shnum*sizeof(int));
	secReorder(file,&shd_o,&hd_o,oldIds);

	writeEHDR(&hd_o,result);
	writeSHDRT(shd_o.tab,shd_o.n,result,hd_o.e_shoff);
	for(int i = 0; i < shd_o.n; i++){
		uint32_t in_offset = old_shd.tab[oldIds[i]].sh_offset;
		copySection(file,result,in_offset,shd_o.tab[i].sh_offset,shd_o.tab[i].sh_size);	
	}	
//	free(oldIds);
//	free(shd_o.tab);
	// Partie 7 : table des symboles
	/*printf("\n******** TESTS **********\n");

	  symbolImplentation(file, &hd_o, &shd_o, oldIds);
	  printf("\n****** FIN TESTS *******\n");
	 */
	fclose(result);
	fclose(file);
	return 0;
}
