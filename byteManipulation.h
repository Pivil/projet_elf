#ifndef __byteManipultaion_h__
#define __byteManipultaion_h__

#include <stdint.h>
#include <stdio.h>

/*
Entrie: the file to read
Result: a byte read
*/
uint8_t get1Byte(FILE *f);

/*
Entrie: the file to read
Result: 2 bytes read
*/
uint16_t get2Bytes(FILE *f,char c);
/*
Entrie: the file to read
Result: 4 bytes read
*/
uint32_t get4Bytes(FILE *f,char c);

/*
Entrie: the file, the byte to write
Result: the byte x written in the file
*/
void print1Byte(FILE *f,uint8_t x);
/*
Entrie: the file, the 2bytes to write,  the endianess 1= little endian, 2 big endian
Result: the 2bytes x written in the file
*/
void print2Bytes(FILE *f,uint16_t x);
/*
Entrie: the file, the 4bytes to write,  the endianess 1= little endian, 2 big endian
Result: the 4bytes x written in the file
*/
void print4Bytes(FILE *f,uint32_t x);
#endif
