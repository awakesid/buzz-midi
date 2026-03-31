#include<stdio.h>
#include"headerparse.h"
#include"helper.h"
#include<string.h>




uint16_t gettrackformat(FILE *fptr){
    return readDivision(readbytes(2,fptr)); // it return the integer of two bytes
}


uint16_t getNumberOftracks(FILE *fptr){
    return readDivision(readbytes(2,fptr)); // it return the integer of two bytes
}

void getheaderinfo(midiheader *head, FILE *fptr){

    strcpy(head->id,readbytes(4, fptr));
    head->length=readLength(readbytes(4,fptr), 4); // it returns the integer of 4 bytes
    head->format=gettrackformat(fptr);
    head->tracks=getNumberOftracks(fptr);
    head->division=readDivision(readbytes(2,fptr));
}



