#include "relocation.h"
#include "string.h"



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
	

	fclose(result);
	result = fopen(argv[2], "r+");

	// Partie 7 : table des symboles
	int addData = 0;
	int addText = 0;

	//Get the parameters of the function
	if (argc == 7) {
		if (strcmp(argv[3], "-data") == 0) 
			addData = getAddress(argv[4]);
		else if (strcmp(argv[5], "-data") == 0)
			addData = getAddress(argv[6]);
		else {
			printf("Veuillez renseigner l'adresse de la section .data (commande -data)\n");
			return 0;
		}

		if (strcmp(argv[3], "-text") == 0)
			addText = getAddress(argv[4]);

		else if (strcmp(argv[5], "-text") == 0)

			addText = getAddress(argv[6]);
		else{
			printf("Veuillez renseigner l'adresse de la section .text (commande -text)\n");
			return 0;
		}

		//Symbol implantation : get the absolue addresse instead of relative addresse
		if (addData != -1 && addText != -1)
			symbolImplantation(result, &hd_o, &shd_o, oldIds, addData, addText);
		else
			printf("Erreur, veuillez entrer une adresse de taille valide\n");
	}

	
	free(oldIds);
	free(shd_o.tab);

	fclose(result);
	fclose(file);
	return 0;
}

