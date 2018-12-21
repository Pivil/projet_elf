#include <stdio.h>
#include "file_1.h"

int main(){ 
 	type_0 t;
	t.str_size = 2;

	int a, b;
	uint32_t x = 32;
	char c ='t';
	a = 99; 
	b = a+1;
	foo_0(a, b);

	foo_1(c, x);

	foo_3(&t);


	return 0;
}
