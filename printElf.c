#include "printElf.h"


void shtToStr(uint32_t sht, char* str){
	switch(sht){
		case SHT_NULL:
			strcpy(str, "NULL");
			break;
		case SHT_PROGBITS:
			strcpy(str,"PROGBITS");
			break;
		case SHT_SYMTAB:
			strcpy(str, "SYMTAB");
			break;
		case SHT_STRTAB:
			strcpy(str, "STRTAB");
			break;
		case SHT_RELA:
			strcpy(str, "RELA");
			break;
		case SHT_HASH:
			strcpy(str, "HASH");
			break;
		case SHT_DYNAMIC:
			strcpy(str, "DYNAMIC");
			break;
		case SHT_NOTE:
			strcpy(str, "NOTE");
			break;
		case SHT_NOBITS:
			strcpy(str, "NOBITS");
			break;
		case SHT_REL:
			strcpy(str, "REL");
			break;
		case SHT_SHLIB:
			strcpy(str, "SHLIB");
			break;
		case SHT_DYNSYM:
			strcpy(str, "DYNSYM");
			break;
		case SHT_LOPROC:
			strcpy(str, "LOPROC");
			break;
		case SHT_HIPROC:
			strcpy(str, "HIPROC");
			break;
		case SHT_LOUSER:
			strcpy(str, "LOUSER");
			break;
		case SHT_HIUSER:
			strcpy(str, "HIUSER");
			break;
		default:
			strcpy(str, "RESERVED");
			break;

	
	}
}

void etToStr(uint16_t e_type, char* str){
	switch(e_type){
		case ET_NONE:
			strcpy(str,"No file type");
			break;
		case ET_REL:
			strcpy(str,"Relocable file");
			break;
		case ET_EXEC:
			strcpy(str,"Executable file");
			break;
		case ET_DYN:
			strcpy(str,"Shared object file");
			break;
		case ET_CORE:
			strcpy(str,"Core file");
			break;
		case ET_LOPROC:
			strcpy(str,"Processor-specific");
			break;
		case ET_HIPROC:
			strcpy(str,"Processor-specific");
			break;

	}
}

void emToStr(uint16_t e_machine,char* str){
	switch(e_machine){
		case EM_NONE:
			strcpy(str,"No machine");
			break;
		case EM_M32:
			strcpy(str,"AT&T WE 32100");
			break;
		case EM_SPARC:
			strcpy(str,"SPARC");
			break;
		case EM_386:
			strcpy(str,"Intel 80386");
			break;
		case EM_68K:
			strcpy(str,"Motorola 68000");
			break;
		case EM_88K:
			strcpy(str,"Motorola 88000");
			break;
		case EM_860:
			strcpy(str,"Intel 80860");
			break;
		case EM_MIPS:
			strcpy(str,"MIPS RS3000");
			break;
		default:
			strcpy(str,"UNKNOWN");
			break;
		
	

	}
}

void print_ehdr(FILE* file, Elf32_Ehdr* ehdr){
	char str[50];
	if(file){

		fread(ehdr,1,sizeof(Elf32_Ehdr),file);
	}


	if(	ehdr->e_ident[0]== 0x7f && 
		ehdr->e_ident[1] == 'E' &&
		ehdr->e_ident[2] == 'L' &&
		ehdr->e_ident[3] == 'F')
	{
		printf("ident : %s\n",ehdr->e_ident);
		etToStr(ehdr->e_type,str);
		printf("type: %s (0x%x)\n",str,ehdr->e_type);
		emToStr(ehdr->e_machine,str);
		printf("machine: %s (0x%x)\n",str,ehdr->e_machine);
		printf("version: %i\n",ehdr->e_version);
		printf("entry: %i\n",ehdr->e_entry);
		printf("phoff: %i\n",ehdr->e_phoff);
		printf("shoff : %i\n",ehdr->e_shoff);
		printf("flags: %i\n",ehdr->e_flags);
		printf("ehsize: %i\n",ehdr->e_ehsize);
		printf("phentsize: %i\n",ehdr->e_phentsize);
		printf("phnum: %i\n",ehdr->e_phnum);
		printf("shentsize: %i\n",ehdr->e_shentsize);
		printf("shnum: %i\n",ehdr->e_shnum);
		printf("shstrndx: %i\n",ehdr->e_shstrndx);


	}
	else{
		printf("NN ELF\n");
	}


}

void print_shdr(FILE* file, SHdrTable* shdr_arg, uint32_t offset, uint16_t hdrSize, uint16_t hdrNb){
	SHdrTable shdr;
	int i;
	char type_str[20];
	shdr = (SHdrTable)malloc(hdrNb*sizeof(Elf32_Shdr));
	if(!shdr){
		printf("err\n");
		return;
	}
	if(file){
		fseek(file, offset, SEEK_SET);

		for(i = 0; i < hdrNb; i++){
			fread(&shdr[i],1,hdrSize,file);
			shtToStr(shdr[i].sh_type,type_str);
			printf("%i  name: %i\ttype: %s (0x%x)\t\tflags: 0x%x\taddr: %i\toffset: %i\tsize: %i\tlink: %i\tinfo: %i\taddralign: %i\tentsize: %i\n",
				i,shdr[i].sh_name,type_str,shdr[i].sh_type,shdr[i].sh_flags,shdr[i].sh_addr,shdr[i].sh_offset,shdr[i].sh_size,shdr[i].sh_link,shdr[i].sh_info,shdr[i].sh_addralign,shdr[i].sh_entsize);
		}

	}
	
	*shdr_arg = shdr;
}

void print_sym(FILE* file, SHdrTable shdr, uint16_t secNum) {

	Elf32_Sym sym;
	uint16_t i=0;
	uint32_t symSize, symOffset;
	

	//on cherche la section de la table des symboles : SHT_SYMTAB 
	while(i < secNum && shdr[i].sh_type != SHT_SYMTAB){
		i++;
	}	

	symSize = shdr[i].sh_size;
	symOffset = shdr[i].sh_offset;
	
	fseek(file,symOffset,SEEK_SET);
	printf("Num\tValeur\tTail\tType\tVis\tNdx\tNom\n");
	for (i = 0; i < symSize/sizeof(Elf32_Sym); i++) {
		fread(&sym,1,sizeof(Elf32_Sym),file);
		printf("%d\t",i);
		printf("%d\t", sym.st_value);
		printf("%d\t", sym.st_size);
		printf("%d\t", sym.st_info);
		printf("%d\t", sym.st_other);
		printf("%d\t", sym.st_shndx);
		printf("%d\t\n", sym.st_name);
	}
}


