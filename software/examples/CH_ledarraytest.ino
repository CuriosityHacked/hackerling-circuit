#include <Wire.h>

#include <MCP23017.h>

#include <MCP23008.h>

MCP23008 mcp23008;
MCP23017 mcp23017; 

void setup() {
 

mcp23017.begin(2);
mcp23008.begin(3);  

        for(int i=0;i<8;i++){
                mcp23008.pinMode(i, 1);// set pin to output
                mcp23008.digitalWrite(i, 1);// set pin to high (turns LED off)
        }
        for(int i=8;i<10;i++){
                mcp23017.pinMode(i, 1);// set pin to output
                mcp23017.digitalWrite(i, 1);// set pin to high (turns LED off)
        }
  
}

void loop() {

    for(int i=0;i<8;i++){
              mcp23008.digitalWrite(i, 0);// set pin to low (turns LED on)
              delay(300);
              mcp23008.digitalWrite(i, 1);// set pin to high (turns LED off)
              delay(300);     
    }
    //now go forward for the first two LEDs

    for(int i=9;i>=8;i--){
              mcp23017.digitalWrite(i, 0);// set pin to low (turns LED on)
              delay(300);
              mcp23017.digitalWrite(i, 1);// set pin to high (turns LED off)
              delay(300);
    }
  
}
