#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
/* ================================================== */
/* Global Variables */
/* ================================================== */
const int  buttonPin_r = 3;    // the pin that the pushbutton is attached to
const int  buttonPin_y = 4;    // the pin that the pushbutton is attached to
const int  buttonPin_g = 5;    // the pin that the pushbutton is attached to
const int  SEQLEN = 10;    // the pin that the pushbutton is attached to
int buttonState_r = 0;         // current state of the button
int buttonState_y = 0;         // current state of the button
int buttonState_g = 0;         // current state of the button
int lastButtonState_r = 0;     // previous state of the button
int lastButtonState_y = 0;     // previous state of the button
int lastButtonState_g = 0;     // previous state of the button
boolean match = false;
unsigned long pattern[SEQLEN] = {4,4,4,4,4,4,4,4,4,4}; /* button pattern, 4 because can't be equals to 0,1,2 */
unsigned long pattern_r[SEQLEN] = {5,5,5,5,5,5,5,5,5,5}; /* LED pattern, 5 because can't be equals to 0,1,2,4 */
int state = 1;					     /* state = length of sequence right now */
int pc = 0;					     /* pattern counter = how many buttons has been pushed */
int game_state = 0;	     /* 0 = listening mode; 1 = button mode */

RF24 radio(9,10);
void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin_r, INPUT);
  pinMode(buttonPin_y, INPUT);
  pinMode(buttonPin_g, INPUT);
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(16);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, 0xC2C2C2C2C2);
  radio.openWritingPipe(0xE7E7E7E7E7);
  radio.setCRCLength(RF24_CRC_16);
  /* radio.setRetries(5,15); */
  printf_begin();
  radio.printDetails();
  Serial.println("Start Listening...");
}
  void loop() {
    if (game_state == 0) {	/* Listening State */
      // Serial.println("Listening:");
      radio.startListening();
      if( radio.available()){
	  /* radio.stopListening(); */
	  /* radio.write(&match, sizeof(match)); */
	  match = false;
	// pc = 0;
	  while (radio.available()) {
	    /* Read the new LED pattern and write the to corresponding array element  */
	    radio.read( (&pattern_r[state-1]),sizeof(unsigned long) ); 
	  }
	  
	  Serial.print("Message received: the pattern is: ");
	  for (int i = 0; i < 10; i++) {
	    Serial.print(pattern_r[i]);
	  }
	  Serial.println("");
	  game_state = 1;
	  radio.stopListening();                                        // First, stop listening so we can talk
      }
    } else if (game_state == 1) {	/* Wait for button push state */
      // read the pushbutton input pin:
      buttonState_r = digitalRead(buttonPin_r);
      buttonState_y = digitalRead(buttonPin_y);
      buttonState_g = digitalRead(buttonPin_g);
      // compare the buttonState to its previous state
      if (buttonState_r != lastButtonState_r) {
	if (buttonState_r == HIGH) {
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
	if (buttonState_y == HIGH) {
	  Serial.print("on_y: ");
	  pattern[pc] = 1;
	  pc++;
	  for (int i = 0; i < pc; i++) {
	    Serial.print(pattern[i]);
	  }
	  Serial.println("");
	}
	delay(50);
      }
      if (buttonState_g != lastButtonState_g) {
	if (buttonState_g == HIGH) {
	  Serial.print("on_g: ");
	  pattern[pc] = 2;
	  pc++;
	  for (int i = 0; i < pc; i++) {
	    Serial.print(pattern[i]);
	  }
	  Serial.println("");
	}
	delay(50);
      }
      // save the current state as the last state, for next time through the loop
      lastButtonState_r = buttonState_r;
      lastButtonState_y = buttonState_y;
      lastButtonState_g = buttonState_g;
      /* ================================================== */
      /* CHECKING if pattern == pattern_r */
      /* ================================================== */
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
	int count = 0;
	while (count < 100) {
	  radio.write(&match, sizeof(match));
	  count ++;
	}
	/* while (!radio.write(&match, sizeof(match) )); */
	Serial.println("Success");
	match = false;
	/* Resetting */
	for (int i = 0; i < SEQLEN; i++) {
	  pattern[i] = 4;
	}

	pc = 0;
	state += 1;
	game_state = 0;
      }
      /* ================================================================================ */
      /* if enought # of buttons have been pushed but pattern != pattern_r */
      /* ================================================================================ */
      if (pc >= state) {
	Serial.println("Pattern UnMatched; Reporting...");
	radio.stopListening();
	int count = 0;
	while (count < 100) {
	  radio.write(&match, sizeof(match));
	  count ++;
	}
	/* while (!radio.write(&match, sizeof(match))); */
	Serial.println("Success");
	/* Resetting */
	for (int i = 0; i < SEQLEN; i++) {
	  pattern[i] = 4;
	  pattern_r[i] = 5;
	}
	pc = 0;
	state = 1;
	game_state = 0;
	Serial.println("Start Listening...");
      }
    }
  }
