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



void printheadervalue(midiheader *head){
    printf("the head is %s\n",head->id);
	printf("the length is %d\n",head->length);
	printf("the format is %d\n",head->format);
	printf("the number of tracks is %d\n",head->tracks);
	printf("the division is %d\n",head->division);
}

int checkcompatability(midiheader *head){
    if(strcmp(head->id, MIDIID) !=0 ){
        printf("Invalid file header ID doesnot match to MThd, id is: %s", head->id);
        return 1;
    }
    
    else if(head->format!=0){
        printf("The midi format is incompatible, format: %d",head->format);
        return 1;
    }

    else if(head->tracks > 1){
        printf("The number of tracks is more than one, number of tracks is %d",head->tracks);
        return 1;
    }

    else{
        printf("Compatible");
        return 0;
    }

}



