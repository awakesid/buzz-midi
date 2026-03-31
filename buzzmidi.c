#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include"headerparse.h"

typedef enum{
	DELTA_TIME,
	STATUS_CODE,
	NOTE,
	VELOCITY
}ParsingState;

ParsingState pstate;

int main(int argc, char*argv[]){

	if(argc!=2){ // argument check
        printf("Usage is: midiparser filename");
    return 1;
    }

	midiheader header;
	FILE *ptr=fopen(argv[1],"r");
	
	if(ptr==NULL){ //file check
        printf("Invalid File");
        return 1;
    }
	getheaderinfo(&header, ptr);
	printheadervalue(&header);
	checkcompatability(&header);


    



return 0;
}
