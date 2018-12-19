#include "byteManipulation.h"

uint16_t get2Bytes(FILE *f) {
    uint16_t x=0; unsigned char c=0;
    fscanf(f,"%c",&c);
    x=c<<8;
    fscanf(f,"%c",&c);
    x=x|c;
    return x;
}

uint32_t get4Bytes(FILE *f) {
    uint32_t x=0; unsigned char c=0;
    fscanf(f,"%c",&c);
    x=c<<24;
    fscanf(f,"%c",&c);
    x=x|(c<<16);
    fscanf(f,"%c",&c);
    x=x|(c<<8);
    fscanf(f,"%c",&c);
    x=x|c;
    return x;
}

void print2Bytes(FILE *f,uint16_t x) {
    fprintf(f,"%c%c",(x>>8) & 0xff,x & 0xff);
}

void print4Bytes(FILE *f,uint32_t x) {
    fprintf(f,"%c%c%c%c",(x>>24) & 0xff,(x>>16) & 0xff,(x>>8) & 0xff,x & 0xff);
}
