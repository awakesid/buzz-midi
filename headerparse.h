

#ifndef HEADER
#define HEADER


#include<stdint.h>

#define MIDIID MIhd

typedef struct
{
    uint32_t length;
    uint16_t format;
    uint16_t tracks;
    uint16_t division;
    char id[4];
}midiheader;


void getheaderinfo(midiheader head, FILE *fptr);






#endif 
