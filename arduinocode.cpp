#include <Arduino.h>
#include "../header/Radiohead-No Surprises.h"

// put function declarations here:
void buzzit(float, float);

void setup() {
  pinMode(22,OUTPUT);
 
}



//test
void loop()
{
  for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++)
  {
    buzzit(durations[i], notes[i]);
  }
  buzzit(3000, 0);
}


// put function definitions here:
void buzzit(float dur, float analog) {
  tone(22,analog,dur);
  
}