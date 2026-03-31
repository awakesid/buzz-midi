#include<stdio.h>
#include"helper.h"
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<byteswap.h>


char *readbytes(int num, FILE *fptr){
	char *string=malloc(num+1);
	for(int i=0; i<num; i++){
	*(string+i)=getc(fptr);
	
	}
	*(string+num)='\0';
	return string;
}


uint32_t readLength(char *lengthBytes,int size){
	uint32_t destination;
	memcpy(&destination,lengthBytes,size);
	return bswap_32(destination);
}

uint16_t readDivision(char *lenB){
	uint16_t destination;
	memcpy(&destination,lenB,2);
	return bswap_16(destination);
}

uint64_t readDeltaTime(FILE *fptr){
	unsigned char byte;
	uint64_t value=0;
	while(1){
	byte=getc(fptr);
        value=value | byte & 0x7F;
        value= value << 7;
	if(byte & 0x80){
          value= value << 7;
	}
	else{
	//pstate=STATUS_CODE;
	return value;
	}
	}
}

