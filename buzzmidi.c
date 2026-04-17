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

	char *filename = argv[1];
	char *slash = strrchr(filename, '/');
	if (slash) filename = slash + 1;

	// Strip .mid extension
	char basename[256];
	strncpy(basename, filename, sizeof(basename) - 1);
	char *dot = strrchr(basename, '.');
	if (dot) *dot = '\0';

	// Build output path
	char outpath[512];
	char outpath1[512];
	snprintf(outpath, sizeof(outpath), "converted/%s.txt", basename);
	snprintf(outpath1, sizeof(outpath1), "header/%s.h", basename);


	FILE *ptr=fopen(argv[1],"r");
	FILE *f = fopen(outpath, "w");
	FILE *f1 = fopen(outpath1,"w");

	if(ptr==NULL){ //file check
        printf("Invalid File");
        return 1;
    }

//-------------------------------------------header-chunk-------------------------------------------------//
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
	 
//---------------------------------------------event-parser-----------------------------------------------//
	while(1){
		readDeltatime(ptr, header.division);
		handle_event(getc(ptr), ptr, f);
		if(break_flag){
			break;
		}
	}
	
make_header(f1);

return 0;
}
