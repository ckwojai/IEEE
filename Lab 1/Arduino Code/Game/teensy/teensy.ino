#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
int game_state = 0;		// 0 = displaying; 1 = transmit/waiting 
int state = 1;			// Sequence state
int sent_state = 0; 		// 0 = wait to be sent; 1 = sent and wait for response
unsigned long match; 	// 0 = doesn't match; 1 = match;
RF24 radio(9,10);
long unsigned seq[10];
const int ledPin_r = 3;
const int ledPin_y = 4;
const int ledPin_g = 5;
int ledState_r = LOW;
int ledState_y = LOW;
int ledState_g = LOW;
long interval = 2500;
unsigned long previousMillis = 0;
int counter = 0;
void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(16);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, 0xE7E7E7E7E7);
  radio.openWritingPipe(0xC2C2C2C2C2);
  radio.setCRCLength(RF24_CRC_16);
  printf_begin();
  radio.printDetails();
  seq[0] = 0; //seq = r y g r y g r y g r
  seq[1] = 1;
  seq[2] = 2;
  seq[3] = 0;
  seq[4] = 1;
  seq[5] = 2;
  seq[6] = 0;
  seq[7] = 1;
  seq[8] = 2;
  seq[9] = 0;
  // for (int i = 0; i < 10; i++) {
  //   seq[i] = rand() % 3;
  // }
  for (int i = 0; i < 10; i++) {
    Serial.print(seq[i]);
  }
  Serial.println("");
  pinMode(ledPin_r, OUTPUT);
  pinMode(ledPin_y, OUTPUT);
  pinMode(ledPin_g, OUTPUT);
  Serial.println(counter);
  Serial.println(state);
}

void loop() {
  if (game_state == 0) {
    /* for (int i = 0; i < state; i++) { */
    /*   Serial.print(seq[i]); */
    /* } */
    unsigned long currentMillis = millis();
    // Turning off LEDs
    if (counter < state) { 
      if(currentMillis - previousMillis > interval) {
	if (counter != 0) {
	  Serial.println("TURNING OFF LEDs");
	  if (seq[counter-1] == 0) {	// Turning OFF the red
	    ledState_r = LOW;
	    digitalWrite(ledPin_r, ledState_r);
	  } else if (seq[counter-1] == 1) { // Turning OFF the yellow
	    ledState_y = LOW;
	    digitalWrite(ledPin_y, ledState_y);
	  } else if (seq[counter-1] == 2) { // Turning OFF the green
	    ledState_g = LOW;
	    digitalWrite(ledPin_g, ledState_g);
	  }
	}
	delay(1000);
	// ==================================================
	// Turning on LEDs
	// ==================================================
	// save the last time you blinked the LED 
	previousMillis = currentMillis;   
	// if the LED is off turn it on and vice-versa:
	if (seq[counter] == 0) {	// Light up the red
	  counter++;
	  ledState_r = HIGH;
	  digitalWrite(ledPin_r, ledState_r);
	  Serial.println("RED UP");
	} else if (seq[counter] == 1) { // Light up the yellow
	  counter++;
	  ledState_y = HIGH;
	  digitalWrite(ledPin_y, ledState_y);
	  Serial.println("Yellow UP");
	} else if (seq[counter] == 2) { // Light up the green
	  counter++;
	  ledState_g = HIGH;
	  digitalWrite(ledPin_g, ledState_g);
	  Serial.println("Green UP");
	}
      }
    } else {
      delay(1000);
      if (seq[counter-1] == 0) {	// Light up the red
	ledState_r = LOW;
	digitalWrite(ledPin_r, ledState_r);
      } else if (seq[counter-1] == 1) { // Light up the yellow
	ledState_y = LOW;
	digitalWrite(ledPin_y, ledState_y);
      } else if (seq[counter-1] == 2) { // Light up the green
	ledState_g = LOW;
	digitalWrite(ledPin_g, ledState_g);
      }
      counter = 0;	
      game_state = 1;
      sent_state = 0;
    }
  }
  // ======================================================================
  // Transmitting
  // ======================================================================
  if (game_state == 1) {
    if (sent_state == 0) {
      Serial.println("Now sending");
      radio.stopListening();
      if (!radio.write( &seq[state-1], sizeof(seq) )){
	Serial.println("Failed");
      }
      else {
	Serial.println("Success");
      }
      sent_state = 1;
    } else if (sent_state == 1) {
      radio.startListening();
      Serial.print("Begin Listening...\n");
      if( radio.available()){                                         // Variable for the received timestamp
	game_state = 0;
	while (radio.available()) {                                   // While there is data ready
	  radio.read( &match, sizeof(match));             
	}
	radio.stopListening();                                        // First, stop listening so we can talk 
	Serial.println(match);
	if (match == 0) {
	  ledState_r = HIGH;
	  digitalWrite(ledPin_r, ledState_r);
	  delay(1000);
	  ledState_r = LOW;
	  digitalWrite(ledPin_r, ledState_r);
	  delay(1000);
	  Serial.println("Not Matched");
	  counter = 0;
	  state = 1;
	} else if (match == 1) {
	  ledState_g = HIGH;
	  digitalWrite(ledPin_g, ledState_g);
	  delay(1000);
	  ledState_g = LOW;
	  digitalWrite(ledPin_g, ledState_g);
	  delay(1000);
	  Serial.println("Matched");
	  counter = 0;
	  state +=1;
	  /* if (state == 10){ */
	  /*   state = 9; */
	  /* } */
	}
      }
      delay(2000); 
    }
    }
}
// if (game_state == 1) {
//   unsigned long msg[10] ={0,1,2};
//   if (!radio.write(&msg, sizeof(unsigned long))){
// 	Serial.println(F("failed"));
//   }
//   // put your main code here, to run repeatedly:
//   delay(2000);
// }


