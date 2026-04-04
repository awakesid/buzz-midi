
#include<stdio.h>
#include"eventparser.h"
#include"helper.h"
#include<string.h>
#include<math.h>
#include<stdbool.h>
#include<byteswap.h>


char trackid[5];
uint32_t tracklength;
float time;
int tempo=500000;
int instrument;
float accummulated_time=0;
bool delay_flag=false;
bool break_flag=false;
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
   
    float tempo_in_sec=(float)tempo/1000000.00;
    float tick = tempo_in_sec/(float)division;
    uint64_t vlq=readVariableLengthQuantity(ptr);
    time=tick*vlq;
}



void meta_events(FILE *ptr){
    
    int length;
    unsigned char type=getc(ptr);
    if(type==TEMPO_TYPE){ 
        int temp_tempo;
        length=getc(ptr);
        memcpy(&temp_tempo,readbytes(length,ptr),length);
        tempo=bswap_32(temp_tempo) >> 8;

    }
    else if(type==BREAK_TYPE){
        break_flag=true;
    }
    else{
        length=getc(ptr);
        skipBytes(ptr,length);
    }
}


void init_previous_bar(){
    previous_bar.time_since_previous_event=0.0;
    previous_bar.event_type=NOTEON_EVENT;
    previous_bar.note=0;
    previous_bar.velocity=0;
}


void build_current_bar(FILE *ptr, unsigned char event){
    current_bar.time_since_previous_event=time;
    current_bar.event_type=event;
    current_bar.note=getc(ptr);
    current_bar.velocity=getc(ptr);
}


void swap(){
    previous_bar.event_type=current_bar.event_type;
    previous_bar.note=current_bar.note;
    previous_bar.time_since_previous_event=current_bar.time_since_previous_event;
    previous_bar.velocity=current_bar.velocity;
}


void compare_bars(FILE *f){
    if(previous_bar.event_type==NOTEON_EVENT && current_bar.event_type==NOTEON_EVENT){
        accummulated_time=accummulated_time+time;// acc=acc+t
        float analog_value=440*pow(2.0,(previous_bar.note-69)/12.0);    //take previous note and convert
        fprintf(f,"buzzit( %f , %f );\n",1000*accummulated_time,analog_value);//take acc time in ms
        accummulated_time=0;   //acc=0
        swap();
    }
    else if (previous_bar.event_type==NOTEON_EVENT && current_bar.event_type==NOTEOFF_EVENT)
    {
        if(previous_bar.note==current_bar.note){
            accummulated_time=accummulated_time+time;//acc=acc+t
            float analog_value=440*pow(2.0,(previous_bar.note-69)/12.0);    //take previous note and convert
            fprintf(f,"buzzit( %f , %f );\n",1000*accummulated_time,analog_value);//take acc time in ms
            accummulated_time=0;//acc=0
            delay_flag=true; //active delay flag
            swap();
        }
        else{
            //acc time
            accummulated_time=accummulated_time+time;
        }
    }
    else if (previous_bar.event_type==NOTEOFF_EVENT && current_bar.event_type==NOTEON_EVENT)
    {
        
        if(delay_flag && time>0){
            
            
            accummulated_time=accummulated_time+time; //acc=acc+t
            fprintf(f,"buzzit( %f , 0 );\n",1000*accummulated_time); //set_delay acc time
            accummulated_time=0;//acc=04
            delay_flag=false;
            
        }
        swap();
    }
    
}

void handle_event(unsigned char ch,FILE *ptr, FILE *f){
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
        compare_bars(f);
        
        break;
    case NOTEON_EVENT:
        build_current_bar(ptr,NOTEON_EVENT);
        compare_bars(f);

        break;

    default:
    int length=getc(ptr);
    skipBytes(ptr,length);
        break;
    }
}