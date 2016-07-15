#include <Wire.h>
#include <MCP23017.h>
#include <MCP23008.h>
#include <LCD.h>

MCP23017 mcp23017;
MCP23008 mcp23008;
LCD lcd = LCD();

// component to pin mapping 
const int led=8;
const int sw1p=4;
const int but3p=4;
const int but2p=5;
const int but4p=6;
const int but1p=7;

// variabes to hold switch state
int but1=0;
int but2=0;
int but3=0;
int but4=0;
int sw1=0;

//player
byte specialChar0[8] = 
  {0,14,17,21,17,14,0,0};
  
//ground 1
byte specialChar1[8] = 
  {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01000
  };
  
//ground 2
byte specialChar2[8] = 
  {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00100
  };

//ground 3
byte specialChar3[8] = 
  {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10010
  };
  
//evil cactus 1
byte specialChar4[8] =
  {
  B00100,
  B10101,
  B10101,
  B11111,
  B00100,
  B00100,
  B00100,
  B00000
  };

//evil cactus 2  
byte specialChar5[8] = 
  {
  B00100,
  B10100,
  B10101,
  B11101,
  B00111,
  B00100,
  B00100,
  B00000
  };

//evil cactus 3
byte specialChar6[8] = 
  {
  B00100,
  B10100,
  B10100,
  B11111,
  B00101,
  B00100,
  B00100,
  B00000
  };

//evil cactus 4
byte specialChar7[8] = 
  {
  B00100,
  B00100,
  B11111,
  B10101,
  B10100,
  B00100,
  B00100
  };

  // player and shot icons
  //const int player = B11101111;
  const int player = 0;
  const int shot = B11101011;

  //initial values
  int counter;
  int ppos;
  int bpos;
  int jump;
  int shots;
  int field[16];

void readSwitches() {
        but1 = mcp23017.digitalRead(but1p);
        but2 = mcp23017.digitalRead(but2p);
        but3 = mcp23017.digitalRead(but3p); 
        but4 = mcp23017.digitalRead(but4p); 
        sw1 = digitalRead(sw1p);
}

void initLCD() {

    lcd.clear();
    lcd.setCursor(0,0);
}

void beep() {
  tone(11, 440, 70);
}

void boop() {
  tone(11,300,100);
}

void clearBargraph(){
          for(int i=0;i<8;i++){
                mcp23008.digitalWrite(i, 1);// set pin to high (turns LED off)
        }
        for(int i=8;i<10;i++){
                mcp23017.digitalWrite(i, 1);// set pin to high (turns LED off)
        }
}

void setBargraph(int a){
  // set lights on bargraph
  if ( a < 8 ) {
      mcp23008.digitalWrite(a, 0);
  }
  if ( a == 8 ){ mcp23017.digitalWrite(9, 0); }
  if ( a == 9 ){ mcp23017.digitalWrite(8, 0); } 

}

void graphTo(int a){
  // set all lights up to
  clearBargraph();
  for(int i=0;i<=a;i++){
    setBargraph(i);
  }
}

void displayCount(){
    int cnt = counter;
    int dgts = 1; 
    while(cnt!=0)
    {
        cnt /= 10;             // n = n/10
        ++dgts;
    }
    lcd.setCursor(17-dgts,0);
    lcd.print(counter);
}

void displayRun() {
  // show the field
  for (int i=0;i<16;i++) {
    if ( i != ppos || jump == 0) {
      lcd.setCursor(i,1);
      lcd.write(field[i]);   
    }
  }
  if (bpos > 0 ){
    lcd.setCursor(bpos,1);
    lcd.write(shot); 
  }
  lcd.setCursor(ppos,jump);
  lcd.write(player);
}

void moveRight() {
  if (ppos < 14) { ++ppos; }
}

void moveLeft() {
  if (ppos > 0) { --ppos; }
}

void shoot() {
  // fire!
  if (shots > 0){
    --shots;
    graphTo(shots-1);
    bpos = ppos + 1;
  }
}
void updateField() {
  for (int i=0; i<15 ; i++){
    field[i] = field[i+1];
  }
  // a blank, ground 1, ground 2, ground 3, a cactus1-4, another blank
  int r = int(random(0,6));
  if ( r == 0 || r == 5) { field[15] = 32; }
  if ( r == 1 ) { field[15] = 1; }
  if ( r == 2 ) { field[15] = 2; }
  if ( r == 3 ) { field[15] = 3; }
  if ( r == 4 ) {
      field[15] = int(random(4,8));
  }
}

void moveShot(){
  if (bpos > 0 && bpos < 14) {
    ++bpos;
    }
    else { bpos = 0; }
}

byte detectCollision(int pos, int jmp) {
    // did we hit something
    if (field[pos] >= 4 && field[pos] < 8 && jmp != 0) {
      return true;
    } else { return false; }
}

void setup() {
  // put your setup code here, to run once:

  // initialize toggle
  pinMode(sw1p, INPUT);

  // initialize IO expander 
  mcp23017.begin(2);  
  mcp23008.begin(3);  

  // tell the switch pins we will be reading from them and set pullUp resistor
  mcp23017.pinMode(but1p, INPUT);
  mcp23017.pinMode(but2p, INPUT);
  mcp23017.pinMode(but3p, INPUT);
  mcp23017.pinMode(but4p, INPUT);
  mcp23017.pullUp(but1p, 1);
  mcp23017.pullUp(but2p, 1);
  mcp23017.pullUp(but3p, 1);
  mcp23017.pullUp(but4p, 1);

// tell bargraph pins we will write to them and set inital state as off
  for(int i=0;i<8;i++){
          mcp23008.pinMode(i, 1);// set pin to output
  }
  for(int i=8;i<10;i++){
          mcp23017.pinMode(i, 1);// set pin to output
  }
  clearBargraph();
  
  // initialize LED
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);

  // initialize LCD
  lcd.begin(16,2);

  lcd.createChar(0,specialChar0);
  lcd.createChar(1,specialChar1);
  lcd.createChar(2,specialChar2);
  lcd.createChar(3,specialChar3);
  lcd.createChar(4,specialChar4);
  lcd.createChar(5,specialChar5);
  lcd.createChar(6,specialChar6);
  lcd.createChar(7,specialChar7);

  initLCD();

  randomSeed(analogRead(0));

  resetGame();

}

void resetGame() {
    counter = 0;
    ppos = 0;
    bpos = 0;
    shots = 5;
    jump = 1;
    for (int i=0; i<16; i++) { 
        int r = int(random(0,5));
        if ( r == 0 || r == 4) { field[i] = 32; }
        if ( r == 1 ) { field[i] = 1; }
        if ( r == 2 ) { field[i] = 2; }
        if ( r == 3 ) { field[i] = 3; }
    }
    lcd.clear();  
    graphTo(shots-1);
}

void loop() {
  // put your main code here, to run repeatedly:  
  readSwitches();
  if ( but2 == 0) {
    if ( jump != 0 ) {
      beep();
    }
    jump = 0;
    }
    else if (jump == 0 ) {
        lcd.setCursor(ppos,jump);
        lcd.write(32);
        jump = 1;
        boop();
    }
  if ( but3 == 0 ) { moveRight(); }
  if ( but1 == 0 ) { moveLeft(); }
  if ( but4 == 0 ) { shoot(); }
  // lcd.setCursor(4,0); lcd.print(but1); lcd.print(but2); lcd.print(but3); lcd.print(but4); lcd.print(sw1); lcd.print(jump); //debug
  // did we shoot something?
  if (bpos > 0 ) {
    if (detectCollision(bpos, 1)) {
        field[bpos] = 32; 
        bpos = 0;
    }
    if (detectCollision(bpos-1, 1)) {
      field[bpos-1] = 32;
      bpos = 0;
    }
  }
  
  // did we run into something?
  if (detectCollision(ppos, jump)) { 
    lcd.setCursor(5,1);
    lcd.print("BOOM!");
    while (but4 == 1) { readSwitches(); } // down button to restart
    resetGame();
  }
  
  displayCount();
  displayRun();

  delay(200);

  updateField();
  moveShot();  
  ++counter;

}
