

#ifndef HEADER
#define HEADER


#include<stdint.h>

#define MIDIID "MThd"

typedef struct
{
    uint32_t length;
    uint16_t format;
    uint16_t tracks;
    uint16_t division;
    char id[5];
}midiheader;


void getheaderinfo(midiheader *head, FILE *fptr); // get all the necessary parameters od the headerchunk
uint16_t gettrackformat(FILE *fptr); // gettrackformat
uint16_t getNumberOftracks(FILE *fptr); // get number of tracks
int checkcompatability(midiheader *head); // check compatability to know that the midi file is convertable or not
void printheadervalue(midiheader *head); //get all the header parameter



#endif 
