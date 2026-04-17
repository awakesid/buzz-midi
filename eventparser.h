#ifndef EVENTPARSER
#define EVENTPARSER
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include<string.h>

#define TRACK_ID "MTrk"
#define TEMPO_TYPE 0x51
#define META_EVENTS 0xFF
#define NOTEON_EVENT 0x90
#define NOTEOFF_EVENT 0x80
#define INSTRU_EVENT 0xC0
#define BREAK_TYPE 0x2F

extern char trackid[5];
extern uint32_t tracklength;
extern float time;
extern int tempo;
extern int instrument;
extern bool delay_flag;
extern bool break_flag;

typedef struct
{
    float time_since_previous_event;
    int note;
    int velocity;
    unsigned char event_type;
}bar;



void GetTrackId(FILE *ptr);
void GetTrackLength(FILE *ptr);
int CheckTrackid();

void readDeltatime(FILE *ptr, int division);
void handle_event(unsigned char ch, FILE *ptr,FILE *f);
void meta_events(FILE *ptr);

void init_previous_bar();
void build_current_bar(FILE *ptr,unsigned char);
void compare_bars(FILE *f);
void swap();

void make_header(FILE *f1);



#endif