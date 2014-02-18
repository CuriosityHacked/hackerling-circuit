/*
Tests directional buttons and provides response on speaker of a v1.3 Hackerling Circuit but also testing lights from the LED array.

Uses direct library calls, to MCP libraries rather then Hackerling Circuit library code.

*/


#include <Wire.h>
#include <MCP23017.h>

MCP23017 mcp23017; 

const int led=8;
const int but3p=4;
const int but2p=5;
const int but4p=6;
const int but1p=7;

// variabes to hold switch state
int but1=0;
int but2=0;
int but3=0;
int but4=0;


void setup() {
  
// init the IO expanders
mcp23017.begin(2);  

// tell the switch pins we will be reading from them and set pullUp resistor

mcp23017.pinMode(but1p, INPUT);
mcp23017.pinMode(but2p, INPUT);
mcp23017.pinMode(but3p, INPUT);
mcp23017.pinMode(but4p, INPUT);
mcp23017.pullUp(but1p, 1);
mcp23017.pullUp(but2p, 1);
mcp23017.pullUp(but3p, 1);
mcp23017.pullUp(but4p, 1);

pinMode(led,OUTPUT);
digitalWrite(led,LOW);

}

void readSwitches() {
        but1 = mcp23017.digitalRead(but1p);
        but2 = mcp23017.digitalRead(but2p);
        but3 = mcp23017.digitalRead(but3p); 
        but4 = mcp23017.digitalRead(but4p); 

}
 
void loop() {

  readSwitches();
   if ( ! but1 ) { tone(11, 262); };
   if ( ! but2 ) { tone(11, 196); };
   if ( ! but3 ) { tone(11, 220); };
   if ( ! but4 ) { tone(11, 247); };
  
   delay(400); //dont run too often

   noTone(11);
}
