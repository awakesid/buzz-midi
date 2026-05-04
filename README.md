# buzz-midi

## How to use? 
First, you can create your own tones using the online sequencer at onlinesequencer.net. If you already have a MIDI file, you can also edit it there. Songs with complex notes can be simplified, or you can extract just the signature melody. It is also recommended to shift the notes around the F5 scale for better sound quality on a buzzer.

1. Clone my buzz-midi repo 
 `git clone https://github.com/awakesid/buzz-midi.git`

2. Go-to buzz-midi folder, open in terminal and run 
`make`

3. Then run command,
`./buzz-midi path/to/midifile.mid`

4. You will find your `midifile.h` file inside header folder containing notes and duration.

5. Now you can simply include this to your project, and for code implementation you can check out `arduinocode.cpp` file.

Using just notes and durations eliminates the need for any heavy audio library, SD card, or external DAC, and it still sounds clean. This makes it a perfect fit for the Deskbot project.

If you want know working of buzz-midi keep reading.

## How it works?
The first step was to understand midi file. [This](https://www.youtube.com/watch?v=P27ml4M3V7A&t=547s) video by [NoBS Code](https://www.youtube.com/@nobs_code) on youtube is one of the best explanation of midi file out there.

So, MIDI(Musical Instrument for Digital Interface) file stores music as instruction, rather than PCM bytes stored in digital audio files WAV, FLAC and MP3. The data are arranged in the form chunks mainly consist of one header chunk and multiple track chunk.

### Header Chunk
It consist of :

**"MThd"** -4 bytes
These four characters at the start of the MIDI file indicate that this  _is_  a MIDI file.

**<header_length>** - 4 bytes
length of the header chunk (always 6 bytes long).

**format**  -2 bytes
*0*  = single track file format  
*1*  = multiple track file format  
*2*  = multiple song file format

**n** -2 bytes
number of tracks that follow

**division**  -2 bytes
unit of time for delta timing.

This implementation only supports MIDI Format 0. Format 1 and Format 2 are intentionally excluded, as both rely on multiple tracks that cannot be mixed down to a single buzzer output. Format 0's single-track structure is sufficient to produce the intended sound.

The file is read byte by byte in C. Helper functions used throughout the parser are defined in `helper.c`.

    #ifndef HELPER
    #define HELPER
    #include<stdio.h>
    #include<stdint.h>
    char *readbytes(int num, FILE *fptr); //fuction to read given number of bytes
    uint32_t readLength(char *lengthBytes,int size); //fucntion to read length from bytes,  it reads 4byte(string) into 32bit unsigned int
    uint16_t readDivision(char *lenB); // fucntion to read division,  it reads 2byte(string) into 16bit unsigned int
    uint64_t readVariableLengthQuantity(FILE *fptr); //function to read delta time (variable lenght quantity)
    void skipBytes(FILE *fptr,int num);
    #endif
    
### Track chunk
It consist of :

**"MTrk"**  4 bytes
the literal string MTrk. This marks the beginning of a track.

**length** - 4 bytes
the number of bytes in the track chunk following this number.

**<track_event>**
a sequenced track event.

Suppose you started parsing the midi file, parsed header chunk,verified file, parsed track chunk id and track length. Now next you will encounter series of **track_event** until the end of track.  A track event can be of three type:
**<v_time>+<midi_event>
<v_time>+<meta_event> 
<v_time>+<sysex_event>**

And every track_event consists of a delta time that represent time since last event.

Our primary goal is to extract notes and duration of each note. So we only focus on midi_events and 2 meta_event. Remaning other events we just skip it by reading its lengths.

First lets talk about 2 meta_events:
**FF**= represent this event is meta_event.
**type=0x2F** represent end of the track. *(0xff 0x2f)*
**type=0x51** represent tempo. After that length byte and acutal tempo.*(0xff 0x51 0x03 + three byte data)*

### Timing control
Extracted tempo, division and delta time from each event is used to calculate the duration of each note. Delta_time actually represent number of tick passed since last event. And single tick is equal to tempo_in_sec/division seconds.
``` math
 tick=tempo_in_sec/division
 ```
 Extracted tempo is in micro-seconds by default, need to be converted to seconds.
But there is another problem. Delta time is varable length quantity(VLQ). Normally it is single byte but can extend up to 2-3 bytes based on duration. If the most significant bit of delta time is 1, continue to read next byte for delta time else stop. This was actually fun part solve VLQ problem in the C. 

    void readDeltatime(FILE *ptr,int division){
        float tempo_in_sec=(float)tempo/1000000.00;
        float tick = tempo_in_sec/(float)division;
        uint64_t vlq=readVariableLengthQuantity(ptr);
        time=tick*vlq;
    }
    
    uint64_t readVariableLengthQuantity(FILE *fptr){
	unsigned char byte;
	uint64_t value=0;
	while(1){
	byte=getc(fptr);
        value=value | (byte & 0x7F);
	if(byte & 0x80){
          value= value << 7;
	}
	else{
	return value;
	}
	}
	}

### Midi_Events
It consist of:

![](https://bloom-passive-f40.notion.site/image/attachment%3A259df992-d154-4ab3-aef2-e1b9aee39416%3Amidievent.png?table=block&id=35558a2c-4cf5-8006-a23e-f19e14eb5a8f&spaceId=53dec56e-e1d5-4233-82a7-ff6d5995530c&width=1420&userId=&cache=v2)


Here 3C is the 60 in decimal and represent *middle c* note. It can be converted to buzzer frequency using formula:

    analog_value=440*pow(2.0,(note-69)/12.0)


Lets look at example of sequence of midi event : 

![](https://bloom-passive-f40.notion.site/image/attachment%3A676080bf-b411-4e72-811f-bc677bb31cbc%3Amidievent2.png?table=block&id=35558a2c-4cf5-80e6-b997-dcf7eeb9f4d7&spaceId=53dec56e-e1d5-4233-82a7-ff6d5995530c&width=1410&userId=&cache=v2)

### Overlapping notes
While reading midi_events sequentially it is generally excepcted simple sequence of note_on and note_off event. 
![](https://bloom-passive-f40.notion.site/image/attachment%3Ac07d66b9-902e-46e1-9e8e-5b44b149b778%3Asequence.png?table=block&id=35658a2c-4cf5-800b-9d41-db3d3316904e&spaceId=53dec56e-e1d5-4233-82a7-ff6d5995530c&width=1410&userId=&cache=v2)
Like palying single note at a time but that is not always the case. There will be overlapping note.
![](https://bloom-passive-f40.notion.site/image/attachment%3Ab0b7e304-7c3d-4dce-8fe1-d35156a0f391%3Aoverloap.png?table=block&id=35658a2c-4cf5-8022-aad4-e7f2b969f710&spaceId=53dec56e-e1d5-4233-82a7-ff6d5995530c&width=1410&userId=&cache=v2)


We need to trim the bar to make it sequential like this and play it on buzzer.

![](https://bloom-passive-f40.notion.site/image/attachment%3Ad1b14a48-a7cc-4553-bcef-12e38369c344%3Acutbar.png?table=block&id=35658a2c-4cf5-8067-80f7-c85be5ceed76&spaceId=53dec56e-e1d5-4233-82a7-ff6d5995530c&width=1040&userId=&cache=v2)
To solve this issue we need to implement buffer previous_bar(previous_event) and current_bar(current_event) and compare them. The bar is structure.

    typedef struct
    {
        float del_time;
        int note;
        int velocity;
        unsigned char event_type;
    }bar;




There will be following orders of events.

    accumalated_time=0;
    void compare_bars(FILE *f){
        if(previous_bar.event_type==NOTEON_EVENT && current_bar.event_type==NOTEON_EVENT){
	       accummulated_time=accummulated_time+current_bar.del_time;
           print previous_bar.note;
           print accumalated_time;
           accummulated_time=0;
            swap();
        }
        else if (previous_bar.event_type==NOTEON_EVENT && current_bar.event_type==NOTEOFF_EVENT)
        {
            if(previous_bar.note==current_bar.note){
						accummulated_time=accummulated_time+current_bar.del_time;
					      print previous_bar.note; 
					      print accumalated_time;
			              accummulated_time=0;//acc=0
		               delay_flag=true; //(can be delay gap between note)
			               swap();
            }
            else{
                accummulated_time=accummulated_time+current_bar.del_time;
            }
        }
        else if (previous_bar.event_type==NOTEOFF_EVENT && current_bar.event_type==NOTEON_EVENT)
        {
            
            if(delay_flag && del_time>0){
                accummulated_time=accummulated_time+current_bar.del_time;
                print accumalated_time;//(delay gap between notes)
                accummulated_time=0;//acc=04
                delay_flag=false;
                
            }
            swap();
        }




