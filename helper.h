#ifndef HELPER
#define HELPER

#include<stdio.h>
#include<stdint.h>

char *readbytes(int num, FILE *fptr); //fuction to read given number of bytes
uint32_t readLength(char *lengthBytes,int size); //fucntion to read length from bytes
uint16_t readDivision(char *lenB); // fucntion to read division 
uint64_t readDeltaTime(FILE *fptr); //function to read delta time


#endif