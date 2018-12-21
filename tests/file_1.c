#include "file_1.h"


void foo_0(int a, int b){

	int k = a+ b;
	k = k << 2;
	
}

int foo_1(char c, uint32_t x){
	if( c == (char)x){
		return 1==1;
	}
	return 1==0;

}

void foo_3(type_0* v){
	if(v->suivant == NULL){
		v->str_size = 0;		
	}
	v->str_size++;

}


