#include "relocation.h"


void secReorder(FILE* input,Elf32_Shdr_seq* shd_o,Elf32_Ehdr* hd_o,int* oldIds ){
	*hd_o = readELFHeader(input);
	Elf32_Shdr_seq shd = readSectionHeader(input,*hd_o);
	uint32_t shoff_off= 0;
	uint32_t old_shoff = hd_o->e_shoff;
	int i,j,k,z;

	uint32_t* rel_off;
	uint32_t* rel_sumSize;
	j=0;k=0;
	shd_o->n = j;
	shd_o->tab = malloc(hd_o->e_shnum*sizeof(Elf32_Shdr));
	for(i = 0; i < hd_o->e_shnum; i++){

		if(shd.tab[i].sh_type != SHT_RELA && shd.tab[i].sh_type != SHT_REL){
			shd_o->tab[j] = shd.tab[i];
			oldIds[j] = i;
			j++;
		}
		else{


			if(k == 0){

				rel_off = malloc(sizeof(uint32_t));
				rel_sumSize = malloc(sizeof(uint32_t));
			}
			else{
				rel_off = realloc(rel_off,(k+1)*sizeof(uint32_t));
				rel_sumSize = realloc(rel_sumSize,(k+1)*sizeof(uint32_t));
			}
			if(rel_off[k] && rel_sumSize[k]){
				if(shd.tab[i].sh_offset >= rel_off[k-1]){
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

	shd_o->n = j-1;
	shd_o->tab = realloc(shd_o->tab,j*sizeof(Elf32_Shdr));
	oldIds = realloc(oldIds,j*sizeof(int));
	for(z = 0; z < j; z++){
		i = 0;
		while(rel_off[i] < shd_o->tab[z].sh_offset && i < k){

			i++;
		}
		i--;
		shd_o->tab[z].sh_offset -= rel_sumSize[i];
	}
	hd_o->e_shnum = j-1;
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

/******************************************************************************/
                            /* Symbol Implentation */
/******************************************************************************/
// *** ETAPE 7 ***
Elf32_Sym_seq symbReorder(FILE* input, Elf32_Shdr_seq* shd_o, Elf32_Ehdr* hd_o, int* oldIds) {
    Elf32_Sym_seq arraySymbol = readSymbolTable(input, *shd_o, *hd_o);
/*
    for(int i = 0; i < arraySymbol.n ; i++) {
    	printf("i : %i  -  tab[i] : %i\n", i, oldIds[i]);

    }
*/
    for (int i = 0; i < arraySymbol.n; i++) { // Pour chaque numéro de section dans la table de symbole
    	for (int j = 0; j < shd_o->n; j++) { // Pour chaque section
    		if (arraySymbol.tab[i].st_shndx == oldIds[j]) {
    			arraySymbol.tab[i].st_shndx = j;
    			printf("Ancien ndx : %i  ||  Nouveau ndx : %i\n", oldIds[j], j); // Renumérotation des numéro de section (shndx) dans la table de symbole

    		}
    	}
    }
    /*
*/
    return arraySymbol;
}



void symbolImplentation(FILE* file, Elf32_Ehdr* ehdr, Elf32_Shdr_seq* arraySection, int* tabCorrespondanceSection) {

    //Renumérotation des sections (shndx)
	Elf32_Sym_seq arraySymbol = symbReorder(file, arraySection, ehdr, tabCorrespondanceSection);
	printf("Debut de l'implentation des symboles\n");
	for (int i = 0; i < arraySymbol.n; i++) {
		for (int j = 0; j < arraySection->n; i++) {
			if (arraySymbol.tab[i].st_shndx == arraySection->tab[j].sh_name) {
				//printf("Mise à jour du symbole %i, ancienne value : %i, nouvelle value : %i (section %i, adresse %i)")
				arraySymbol.tab[i].st_value += arraySection->tab[i].sh_addr // TODO : Demander pourquoi les sh_addr sont
																					//tous à 0 dans la table des sections
			}
		}
	}


}
