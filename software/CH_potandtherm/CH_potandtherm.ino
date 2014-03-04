#include <Wire.h>
#include <math.h>
#include <MCP23017.h>
#include <MCP23008.h>

MCP23008 mcp23008;
MCP23017 mcp23017; 

int delaylength = 300;

#define ThermistorPIN 2                 // Analog Pin 0

float vcc = 4.91;                       // only used for display purposes, if used
                                        // set to the measured Vcc.
float pad = 9850;                       // balance/pad resistor value, set this to
                                        // the measured resistance of your pad resistor
float thermr = 10000;                   // thermistor nominal resistance



void setup() {
  
    Serial.begin(9600);
  
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

    for(int i=0;i<=9;i++){
              delaylength = analogRead(3);
              setBar(i, 0);// set pin to low (turns LED on)
              delay(delaylength);
              setBar(i, 1);// set pin to high (turns LED off)
              delay(delaylength);          
    }
    
    Serial.print("Therm: "); 
    float x = map(analogRead(ThermistorPIN),700,250,14,441);  // analog pin reads 250-700, corresponds to 1.4C to 44.1C
    x /= 10.0;          // divide by 10; map() uses integers
    x = (x * 9.0)/ 5.0 + 32.0;
    Serial.println(x);  // output to serial
   
  
  
}

void setBar(int LED, boolean s){
  if (LED < 8) { mcp23008.digitalWrite(LED, s); }// set pin to low (turns LED on) 
  if ( LED == 8 ) { mcp23017.digitalWrite(9, s); }// set pin to low (turns LED on) 
  if ( LED == 9 ) { mcp23017.digitalWrite(8, s); }// set pin to low (turns LED on)
}




float Thermistor(int RawADC) {
  long Resistance;  
  float Temp;  // Dual-Purpose variable to save space.

  Resistance=pad*((1024.0 / RawADC) - 1); 
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
  Temp = (Temp * 9.0)/ 5.0 + 32.0;                  // Convert to Fahrenheit
  return Temp;                                      // Return the Temperature
}


