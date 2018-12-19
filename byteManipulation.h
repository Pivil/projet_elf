#ifndef __byteManipultaion_h__
#define __byteManipultaion_h__

#include <stdint.h>
#include <stdio.h>

uint16_t get2Bytes(FILE *f);
uint32_t get4Bytes(FILE *f);
void print2Bytes(FILE *f,uint16_t x);
void print4Bytes(FILE *f,uint32_t x);
#endif
