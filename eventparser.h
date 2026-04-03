#ifndef EVENTPARSER
#define EVENTPARSER
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#define TRACK_ID "MTrk"
#define TEMPO_TYPE 0x51
#define META_EVENTS 0xFF
#define NOTEON_EVENT 0x80
#define NOTEOFF_EVENT 0x90
#define INSTRU_EVENT 0xC0

extern char trackid[5];
extern uint32_t tracklength;
extern int time;
extern int tempo;
extern int instrument;

typedef struct
{
    int time_since_previous_event;
    int note;
    int velocity;
    char event_type;
}bar;


void GetTrackId(FILE *ptr);
void GetTrackLength(FILE *ptr);
int CheckTrackid();

void readDeltatime(FILE *ptr, int division);
void handle_event(unsigned char ch, FILE *ptr);
void meta_events(FILE *ptr);

void build_current_bar(FILE *ptr);





#endif