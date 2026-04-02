#ifndef EVENTPARSER
#define EVENTPARSER
#include<stdio.h>
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


void GetTrackId(FILE *ptr);
void GetTrackLength(FILE *ptr);

int CheckTrackid();


#endif