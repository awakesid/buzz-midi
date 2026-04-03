
#include<stdio.h>
#include"eventparser.h"
#include"helper.h"
#include<string.h>
#include<stdbool.h>


char trackid[5];
uint32_t tracklength;
int time;
int tempo=500000;
int instrument;
int accummulated_time=0;
bool delay_flag;

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


void compare_bars(){
    if(previous_bar.event_type==NOTEON_EVENT && current_bar.event_type==NOTEON_EVENT){
        //acc=acc+t
        //take previous note
        //take acc time
        //acc=0
        //swap
    }
    else if (previous_bar.event_type==NOTEON_EVENT && current_bar.event_type==NOTEOFF_EVENT)
    {
        if(previous_bar.note==current_bar.note){
            delay_flag=true;
            //acc=acc+t
            //store previous note
            //store acc time 
            //acc=0
            //active delay flag
        }
        else{
            //acc time
            accummulated_time=accummulated_time+time;
        }
    }
    else if (previous_bar.event_type==NOTEOFF_EVENT && current_bar.event_type==NOTEON_EVENT)
    {
        if(delay_flag && time>0){
            //set_delay
        }
        else{
            //accumulate time
            accummulated_time=accummulated_time+time;
            //take previous note and accumulated time
            //acc=0
            //swap
        }
    }
    else if (previous_bar.event_type==NOTEOFF_EVENT && current_bar.event_type==NOTEOFF_EVENT)
    {
        //no case
    }
    
    
    

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
        build_current_bar(ptr,NOTEOFF_EVENT);
        compare_bars();
        
        break;
    case NOTEON_EVENT:
        build_current_bar(ptr,NOTEON_EVENT);
        compare_bars();

        break;

    default:
    int length=getc(ptr);
    skipBytes(ptr,length);
        break;
    }
}