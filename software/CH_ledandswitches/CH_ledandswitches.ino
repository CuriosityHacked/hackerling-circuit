/*
Tests big switch and 3dip switches of a v1.3 Hackerling Circuit but also testing lights from the LED array.

Uses direct library calls, to MCP libraries rather then Hackerling Circuit library code.

*/


#include <Wire.h>
#include <MCP23017.h>
#include <MCP23008.h>

MCP23008 mcp23008;
MCP23017 mcp23017; 

// define pin locations for switches and lights
int led=8;
int sw1p=4;
int dip2p=3;
int dip3p=2;
int dip4p=1;

// variabes to hold switch state
int sw1=0;
int dip2=0;
int dip3=0;
int dip4=0;


void setup() {
  
// init the IO expanders
mcp23017.begin(2);
mcp23008.begin(3);  

// tell the switch pins we will be reading from them and set pullUp resistor
pinMode(sw1p, INPUT);
mcp23017.pinMode(dip2p, INPUT);
mcp23017.pinMode(dip3p, INPUT);
mcp23017.pinMode(dip4p, INPUT);
mcp23017.pullUp(dip2p, 1);
mcp23017.pullUp(dip3p, 1);
mcp23017.pullUp(dip4p, 1);

// tell LED pins we will write to them and set inital state as off
        pinMode(led, OUTPUT);

        for(int i=0;i<8;i++){
                mcp23008.pinMode(i, 1);// set pin to output
                mcp23008.digitalWrite(i, 1);// set pin to high (turns LED off)
        }
        for(int i=8;i<10;i++){
                mcp23017.pinMode(i, 1);// set pin to output
                mcp23017.digitalWrite(i, 1);// set pin to high (turns LED off)
        }
        digitalWrite(led, HIGH);   // turn the LED off
        
 // blink to show program loaded and ready to roll       
        for(int i=0;i<4;i++){
          allSet(LOW);
          delay(400);
          allSet(HIGH);
          delay(400);
        }
}

void readSwitches() {
        sw1 = digitalRead(sw1p);
        dip2 = mcp23017.digitalRead(dip2p);
        dip3 = mcp23017.digitalRead(dip3p);
        dip4 = mcp23017.digitalRead(dip4p); 
}

void allSet(int a) {
    for(int i=0;i<8;i++){
              mcp23008.digitalWrite(i, a);
    }
    for(int i=9;i>=8;i--){
              mcp23017.digitalWrite(i, a);
    }     
    digitalWrite(led, a);     
}
 
void loop() {

  readSwitches();
       
      digitalWrite(led, sw1);

      // turn on two bar lights per switch
      mcp23017.digitalWrite(9, dip2);
      mcp23008.digitalWrite(7, dip2);

      mcp23008.digitalWrite(5, dip3);
      mcp23008.digitalWrite(4, dip3);      

      mcp23008.digitalWrite(2, dip4);
      mcp23008.digitalWrite(1, dip4);
 
  delay(1000); //dont run too often

}
