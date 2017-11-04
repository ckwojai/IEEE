#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
/*
  State change detection (edge detection)

  Often, you don't need to know the state of a digital input all the time, but
  you just need to know when the input changes from one state to another.
  For example, you want to know when a button goes from OFF to ON. This is called
  state change detection, or edge detection.

  This example shows how to detect when a button or button changes from off to on
  and on to off.

  The circuit:
  - pushbutton attached to pin 2 from +5V
  - 10 kilohm resistor attached to pin 2 from ground
  - LED attached from pin 13 to ground (or use the built-in LED on most
    Arduino boards)

  created  27 Sep 2005
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ButtonStateChange
*/

// this constant won't change:
const int  buttonPin_r = 3;    // the pin that the pushbutton is attached to
const int  buttonPin_y = 4;    // the pin that the pushbutton is attached to
const int  buttonPin_g = 5;    // the pin that the pushbutton is attached to
// const int ledPin = 13;       // the pin that the LED is attached to

// Variables will change:

int buttonPushCounter_r = 0;   // counter for the number of button presses
int buttonPushCounter_y = 0;   // counter for the number of button presses
int buttonPushCounter_g = 0;   // counter for the number of button presses
int buttonState_r = 0;         // current state of the button
int buttonState_y = 0;         // current state of the button
int buttonState_g = 0;         // current state of the button
int lastButtonState_r = 0;     // previous state of the button
int lastButtonState_y = 0;     // previous state of the button
int lastButtonState_g = 0;     // previous state of the button

boolean match = false;
unsigned long pattern[10];
unsigned long pattern_r[10] = {4,4,4,4,4,4,4,4,4,4};
// unsigned long pattern_r[10];
int state = 1;
int game_state = 0; 		/* 0 = listening mode; 1 = button mode */

int pc = 0;
RF24 radio(9,10);
void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin_r, INPUT);
  pinMode(buttonPin_y, INPUT);
  pinMode(buttonPin_g, INPUT);
  // initialize the LED as an output:
  // pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(20);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, 0xC2C2C2C2C2);
  radio.openWritingPipe(0xE7E7E7E7E7);
  radio.setCRCLength(RF24_CRC_16);
  printf_begin();
  radio.printDetails();
  for (int i = 0; i < 10; i++) {
    Serial.print(pattern_r[i]);
    } 
  Serial.println("");
  for (int i = 0; i < 10; i++) {
    Serial.print(pattern[i]);
  } 
}



void loop() {
  if (game_state == 0) {
  // read the pushbutton input pin:
  radio.startListening();
    if( radio.available()){
      match = false;
      game_state == 1;
      // pc = 0;
      // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &pattern_r, sizeof(unsigned long) );             // Get the payload
      }
      Serial.print("Message received: the pattery is\n");
      radio.stopListening();                                        // First, stop listening so we can talk 
      for (int i = 0; i < 10; i++) {
	Serial.print(pattern_r[i]);
      } 
    }
  } else if (game_state == 0) {
  buttonState_r = digitalRead(buttonPin_r);
  buttonState_y = digitalRead(buttonPin_y);
  buttonState_g = digitalRead(buttonPin_g);
  // compare the buttonState to its previous state
  if (buttonState_r != lastButtonState_r) {
    // if the state has changed, increment the counter
    if (buttonState_r == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter_r++;
      Serial.print("on_r: ");
      pattern[pc] = 0;
      pc++;
      for (int i = 0; i < pc; i++) {
	Serial.print(pattern[i]);
      } 

      Serial.println("");
    } 
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  if (buttonState_y != lastButtonState_y) {
    // if the state has changed, increment the counter
    if (buttonState_y == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter_y++;
      Serial.print("on_y: ");
      pattern[pc] = 1;
      pc++;
      for (int i = 0; i < pc; i++) {
	Serial.print(pattern[i]);
      } 
      Serial.println("");
    }

    // Delay a little bit to avoid bouncing
    delay(50);
  }
  if (buttonState_g != lastButtonState_g) {
    // if the state has changed, increment the counter
    if (buttonState_g == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter_g++;
      Serial.print("on_g: ");
      pattern[pc] = 2;
      pc++;      
      for (int i = 0; i < pc; i++) {
	Serial.print(pattern[i]);
      } 
      Serial.println("");
    } 

    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState_r = buttonState_r;
  lastButtonState_y = buttonState_y;
  lastButtonState_g = buttonState_g;
  // ==================================================
  // CHECKING if pattern == pattern_r
  // ==================================================
  boolean temp = true;
  for (int i = 0; i < state; i++) {
    if (pattern[i] != pattern_r[i]) {
	temp = false;
      }
  }
  match = temp;
  
  if (match == true) {
    Serial.println("Pattern Matched; Reporting...");
    radio.stopListening();
     if (!radio.write(&match, sizeof(match) )){
       Serial.println("Failed");
     }   
     match = false;
     // reseting
     for (int i = 0; i < pc; i++) {
       pattern[i] = 4;
     }
     pc = 0;
     state += 1;
     game_state = 0;
  }
  // ==================================================
  // CHECKING if pattern == pattern_r
  // ==================================================
  if (pc >= state) {
    Serial.println("Pattern UnMatched; Reporting...");
    radio.stopListening();
    if (!radio.write(&match, sizeof(match) )){
       // Serial.println("Failed");
    }   
    pc = 0;
    state = 1;
    game_state = 0;
  }
  }
}