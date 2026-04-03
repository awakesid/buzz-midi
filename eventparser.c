
#include<stdio.h>
#include"eventparser.h"
#include"helper.h"
#include<string.h>


char trackid[5];
uint32_t tracklength;
int time;
int tempo=500000;
int instrument;

bar current_bar;
bar previous_bar;

void GetTrackId(FILE *ptr){
    strcpy(trackid,readbytes(4,ptr));
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

void readDeltatime(FILE *ptr,int division){
    int tempo_in_sec=tempo/1000000;
    int tick = tempo_in_sec/division;
    time=tick*readVariableLengthQuantity(ptr);
}



void meta_events(FILE *ptr){
    int length;
    if(getc(ptr)==TEMPO_TYPE){
        int temp_tempo;
        length=getc(ptr);
        memcpy(temp_tempo,readbytes(length,ptr),length);
        tempo=temp_tempo;
    }
    else{
        length=getc(ptr);
        skipBytes(ptr,length);
    }
}


void build_current_bar(FILE *ptr, unsigned char event){
    current_bar.time_since_previous_event=time;
    current_bar.event_type=event;
    current_bar.note=getc(ptr);
    current_bar.velocity=getc(ptr);

}

void handle_event(unsigned char ch,FILE *ptr){
    switch (ch)
    {
    case META_EVENTS:
        meta_events(ptr);
        break;
    case INSTRU_EVENT:
        instrument=getc(ptr);
        break;
    case NOTEOFF_EVENT:
        
        
        break;
    case NOTEON_EVENT:
        build_current_bar(ptr,NOTEON_EVENT);

        break;

    default:
    int length=getc(ptr);
    skipBytes(ptr,length);
        break;
    }
}