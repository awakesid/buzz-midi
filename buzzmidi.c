#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include"headerparse.h"
#include"eventparser.h"



int main(int argc, char*argv[]){


	if(argc!=2){ // argument check
        printf("Usage is: midiparser filename");
    return 1;
    }

	midiheader header;
	FILE *ptr=fopen(argv[1],"r");

	FILE *f = fopen("output.txt", "w");
	
	
	if(ptr==NULL){ //file check
        printf("Invalid File");
        return 1;
    }
	getheaderinfo(&header, ptr);
	printheadervalue(&header);
	checkcompatability(&header);

//-------------------------------------------track-chunk-------------------------------------------------//
	GetTrackId(ptr);
	GetTrackLength(ptr);

	init_previous_bar();

	if(!CheckTrackid()){
		printf("Invalid Track Id, id:%s\n",trackid);
		return 1;
	}


	while(1){
		readDelta_time(ptr, header.division);
		handle_event(getc(ptr), ptr, f);
		if(break_flag){
			break;
		}
	}
	free(ptr);
	free(f);


return 0;
}
