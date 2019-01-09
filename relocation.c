#include "relocation.h"
#include <string.h>


void secReorder(FILE* input,Elf32_Shdr_seq* shd_o,Elf32_Ehdr* hd_o,int* oldIds ){
	*hd_o = readELFHeader(input);
	Elf32_Shdr_seq shd = readSectionHeader(input,*hd_o);
	uint32_t shoff_off= 0;
	uint32_t old_shoff = hd_o->e_shoff;
	int i,j,k,z;
	int idShstr = -1;
	uint32_t* rel_off = malloc(hd_o->e_shnum*sizeof(uint32_t));
	uint32_t* rel_sumSize = malloc(hd_o->e_shnum*sizeof(uint32_t));
	j=0;k=0;
	shd_o->n = j;
	shd_o->tab = malloc(hd_o->e_shnum*sizeof(Elf32_Shdr));
	for(i = 0; i < hd_o->e_shnum; i++){
		if(shd.tab[i].sh_type != SHT_RELA && shd.tab[i].sh_type != SHT_REL){
			shd_o->tab[j] = shd.tab[i];
			if(i == hd_o->e_shstrndx){
				idShstr = j;
			}

			oldIds[j] = i;
			j++;
		}
		else{


			if(rel_off && rel_sumSize){

				if(k == 0){
					rel_off[k] = shd.tab[i].sh_offset;
					rel_sumSize[k] = shd.tab[i].sh_size;
				}
				else if(shd.tab[i].sh_offset >= rel_off[k-1]){
					if(shd.tab[i].sh_size > 0){
						rel_off[k] = shd.tab[i].sh_offset;
						rel_sumSize[k] = rel_sumSize[k-1]+shd.tab[i].sh_size;

					}
				}
				else{
					z=k-1;
					while(rel_off[z] > shd.tab[i].sh_offset){
						z--;
					}
					uint32_t tmp, tmpp;
					uint32_t s_tmp, s_tmpp;

					tmp = rel_off[++z];
					rel_off[z] = shd.tab[i].sh_offset;
					s_tmp = rel_sumSize[z];
					rel_sumSize[z] = rel_sumSize[z] + shd.tab[i].sh_size;

					for(z=z+1; z < k; z++){
						tmpp = rel_off[z];
						rel_off[z] = tmp;
						tmp = tmpp;

						s_tmpp = rel_sumSize[z];
						rel_sumSize[z] = s_tmp;
						s_tmp = s_tmpp + shd.tab[i].sh_size;


					}
					rel_off[z] = tmp;

				}
				k++;
			}


			if(shd.tab[i].sh_offset < old_shoff){
				shoff_off += shd.tab[i].sh_size;
			}
		}
	}
	rel_off = realloc(rel_off,k*sizeof(uint32_t));
	rel_sumSize = realloc(rel_sumSize,k*sizeof(uint32_t));

	shd_o->n = j;
	shd_o->tab = realloc(shd_o->tab,j*sizeof(Elf32_Shdr));
	oldIds = realloc(oldIds,j*sizeof(int));
	for(z = 0; z < j; z++){
		i = -1;
		while(rel_off[i+1] < shd_o->tab[z].sh_offset && i+1 < k){

			i++;
		}

		if(shd_o->tab[z].sh_link != 0){
			for(int w = 0; w < j; w++){
				if((unsigned int)oldIds[w] == shd_o->tab[z].sh_link){
					shd_o->tab[z].sh_link = w;
				}
			}
		}		
		if(i >= 0){
			shd_o->tab[z].sh_offset -= rel_sumSize[i];			

		}
	}
	if(idShstr != -1){
		hd_o->e_shstrndx = idShstr ;
	}
	hd_o->e_shnum = j;
	hd_o->e_shoff -= shoff_off;
	free(rel_sumSize);
	free(rel_off);
}

void writeEHDR(Elf32_Ehdr* hd, FILE* output){
	fseek(output,0,SEEK_SET);
	fwrite(hd,sizeof(Elf32_Ehdr),1,output);
}


void writeSHDRT(Elf32_Shdr* shdt,int shdtSize, FILE* output, uint32_t offset){
	resizeFile(output,offset,shdtSize*sizeof(Elf32_Shdr));
	fseek(output,offset,SEEK_SET);
	fwrite(shdt,sizeof(Elf32_Shdr),shdtSize,output);

}

void copySection(FILE* input, FILE* output, uint32_t in_offset,uint32_t out_offset, uint32_t size){
	resizeFile(output,out_offset,size);
	fseek(output,out_offset,SEEK_SET);
	fseek(input,in_offset,SEEK_SET);
	unsigned char c;
	for(uint32_t i = 0; i < size; i++){
		fscanf(input,"%c",&c);
		fprintf(output,"%c",c);
	}
}

void resizeFile(FILE* file, uint32_t offset, uint32_t size){

	if(size != 0){
		uint32_t sz = 0;
		fseek(file,0L,SEEK_END);
		sz = ftell(file);
		int diff = sz - (offset+size);
		if( diff < 0){
			unsigned char c = 0;
			for(int i = 0; i < diff; i++){
				fprintf(file,"%c",c);
			}
		}
	}
}

void changeAddressSec(Elf32_Shdr_seq* shd_o, int id, uint32_t address){
	if(id < shd_o->n){
		shd_o->tab[id].sh_addr = address;
	}
}



/******************************************************************************/
/* Symbol Implentation */
/******************************************************************************/


// *** ETAPE 7 ***
int getAddress(char* address) { //Get the address of a commande like 0x200 (get just '200')
	if (strlen(address) < 3 || strlen(address) > 10) //Return -1 if the lenght is too short / too long
		return -1;
	if (address == NULL || address[0] != '0' || address[1] != 'x') {
		printf("Erreur, adresse incorrecte\n");
		return 0;
	} 
	else {
		address = &address[2];
		return(atoi(address));
	}
}

void writeSYMB (Elf32_Shdr_seq tabShdr, Elf32_Sym_seq* symboles, FILE* result){
	Elf32_Off offset;
	for (int i = 0; i < tabShdr.n; i++) {
		if (tabShdr.tab[i].sh_type == SHT_SYMTAB) {
			offset = tabShdr.tab[i].sh_offset; // Search the symbol section offset
		}
	}

	fseek(result, offset, SEEK_SET);
	fwrite(symboles->tab, sizeof(Elf32_Sym), symboles->n, result); //Override old information in the result file
}

void symbolImplantation(FILE* result, Elf32_Ehdr* ehdr, Elf32_Shdr_seq* arraySection, int* oldIds, int addData, int addText) {
	Elf32_Sym_seq arraySymbol = readSymbolTable(result, *arraySection, *ehdr);
	Elf32_Addr symbValue;
	Elf32_Half symbShndx;
	char* sectionName;
    for (int i = 0; i < arraySymbol.n; i++) { // For each symbol
    	symbValue = arraySymbol.tab[i].st_value;
    	symbShndx = arraySymbol.tab[i].st_shndx;

    	for (int j = 0; j < arraySection->n; j++) { // Update the section id
    		if (symbShndx == oldIds[j]) {
    			symbShndx = j;
				sectionName = getSectionName(*arraySection, j, *ehdr, result);

    		}
    		if (symbShndx == j) {
				if (strcmp(sectionName, ".text") == 0) {
					arraySymbol.tab[i].st_value += addText; // Update the symbol value field (st_value) by adding the .text memory address
				}

				if (strcmp(sectionName, ".data") == 0) {
					arraySymbol.tab[i].st_value += addData; //Update the symbol value field (st_value) by adding the .data memory address

				} 		
			}
		}
	}	


    free(sectionName);  


    writeSYMB(*arraySection, &arraySymbol, result);
}

