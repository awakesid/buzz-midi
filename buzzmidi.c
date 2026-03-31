#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<byteswap.h>

typedef enum{
	DELTA_TIME,
	STATUS_CODE,
	NOTE,
	VELOCITY
}ParsingState;


ParsingState pstate;


//main function

int main(int argc, char*argv[]){

// argument check

    if(argc!=2){
        printf("Usage is: midiparser filename");
    return 1;
    }

    FILE *ptr=fopen(argv[1],"r");

//file check
    if(ptr==NULL){
        printf("Invalid File");
        return 1;
    }



char *headerID;
uint32_t headerLength=0;
uint16_t division;

headerID=readstring(4,ptr);
puts(headerID);

// header check
if(strcmp(headerID,"MThd")!=0){
printf("Invalid File. Plz provide midi file");
return 1;
}


headerLength=readLength(readstring(4,ptr),4);
printf("the lenght is %d\n",headerLength);

for(int i=0; i<headerLength-2; i++){
    getc(ptr);
}



division=readDivision(readstring(2,ptr));
SecondIntick=tempoINsec/division;

char *trackID=readstring(4,ptr);
puts(trackID);

uint32_t trackLength=readLength(readstring(4,ptr),4);
printf("the track length id %d \n", trackLength);

if (strcmp(trackID,"MTkr")==0){
    pstate=DELTA_TIME;	
   for(int i=0; i<=trackLength; i++){
	switch (pstate)
	{
	case DELTA_TIME:
		printf("the time is %f", secondINtick*readDeltaTime(ptr));
		break;
	case STATUS_CODE:

		break;
	case NOTE:
		break;
	case VELOCITY:
		break;
	}	
      
   }

}
else{



}

// read main header



//while((byte=getc(ptr)) != EOF){

//printf("0x%02X ",byte);
//}
return 0;
}
