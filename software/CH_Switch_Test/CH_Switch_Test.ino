/*
Tests big switch of a v1.3 Hackerling Circuit using the main LED.

Curiosity Hacked
*/


// define pin locations for switches and lights
int led=8;
int sw1p=4;

void setup() {
  
// tell the switch pins we will be reading from them 
pinMode(sw1p, INPUT);


// tell LED pins we will write to them and set inital state as off
pinMode(led, OUTPUT);

digitalWrite(led, HIGH);   // turn the LED off
}

void loop() {

      digitalWrite(led, digitalRead(sw1p));

  delay(500); //dont run too often

}
