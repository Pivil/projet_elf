#include "byteManipulation.h"
#include <stdlib.h>

uint8_t get1Byte(FILE *f) {
    unsigned char c=0; int a=0;
    a=fscanf(f,"%c",&c);
    if(a==0) {
        exit(0);
    }
    return c;
}

uint16_t get2Bytes(FILE *f,char endian) {
    uint16_t x=0; unsigned char c=0;
    if(endian==1) { //Little endian
        fscanf(f,"%c",&c);
        x=c;
        fscanf(f,"%c",&c);
        x=x|(c<<8);
    }
    if(endian==2) { //Big endian
        fscanf(f,"%c",&c);
        x=c<<8;
        fscanf(f,"%c",&c);
        x=x|c;
    }
    return x;
}

uint32_t get4Bytes(FILE *f,char endian) {
    uint32_t x=0; unsigned char c=0;
    if(endian==2) {
        fscanf(f,"%c",&c);
        x=c<<24;
        fscanf(f,"%c",&c);
        x=x|(c<<16);
        fscanf(f,"%c",&c);
        x=x|(c<<8);
        fscanf(f,"%c",&c);
        x=x|c;
    }
    if(endian==1) { //Little endian
        fscanf(f,"%c",&c);
        x=c;
        fscanf(f,"%c",&c);
        x=x|(c<<8);
        fscanf(f,"%c",&c);
        x=x|(c<<16);
        fscanf(f,"%c",&c);
        x=x|(c<<24);
    }
    return x;
}
void print1Byte(FILE *f,uint8_t x) {
    fprintf(f,"%c",x);
}

void print2Bytes(FILE *f,uint16_t x,char endian) {
    if(endian==1) { //Little endian
        fprintf(f,"%c%c",x & 0xff,(x>>8) & 0xff);
    }
    else if(endian==2) { //Bif endian
        fprintf(f,"%c%c",(x>>8) & 0xff,x & 0xff);
    }
}

void print4Bytes(FILE *f,uint32_t x, char endian) {
    if(endian==1) { //Little endian
        fprintf(f,"%c%c%c%c",x>> & 0xff,(x>>8) & 0xff,(x>>16) & 0xff,(x>>24) & 0xff);
    }
    else if(endian==2) {
        fprintf(f,"%c%c%c%c",(x>>24) & 0xff,(x>>16) & 0xff,(x>>8) & 0xff,x & 0xff);
    }
}
