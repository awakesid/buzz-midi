
#include<stdio.h>
#include"eventparser.h"
#include"helper.h"
#include<string.h>


char trackid[5];
uint32_t tracklength;


void GetTrackId(FILE *ptr){
    strcpy(trackid,readbytes(4,ptr));
    printf("\nll%s\n",trackid);
}

void GetTrackLength(FILE *ptr){
    tracklength=readLength(readbytes(4,ptr),4);
}

int CheckTrackid(){
    if(strcmp(trackid,TRACK_ID) != 0){
       
        return 0;
        
    }
    return 1;
}


