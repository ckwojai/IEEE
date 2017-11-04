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
  radio.setChannel(20);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, 0xC2C2C2C2C2);
  radio.openWritingPipe(0xE7E7E7E7E7);
  radio.setCRCLength(RF24_CRC_16);
  radio.setRetries(15,15);  
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
    Serial.println(seq[i]);
  }
  pinMode(ledPin_r, OUTPUT);
  pinMode(ledPin_y, OUTPUT);
  pinMode(ledPin_g, OUTPUT);

}

void loop() {
  if (game_state == 0) {
    unsigned long currentMillis = millis();
    // Turning off LEDs
    if (counter < state) { 
      if(currentMillis - previousMillis > interval) {
	if (counter != 0) {
	  Serial.println("TURNING OFF LEDs");
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
	}
	delay(1000);
	// ==================================================
	// Turning on LEDs
	// ==================================================
	Serial.println("TURNING ON LEDs");
	// save the last time you blinked the LED 
	previousMillis = currentMillis;   
	// if the LED is off turn it on and vice-versa:
	if (seq[counter] == 0) {	// Light up the red
	  counter++;
	  ledState_r = HIGH;
	  digitalWrite(ledPin_r, ledState_r);
	} else if (seq[counter] == 1) { // Light up the yellow
	  counter++;
	  ledState_y = HIGH;
	  digitalWrite(ledPin_y, ledState_y);
	} else if (seq[counter] == 2) { // Light up the green
	  counter++;
	  ledState_g = HIGH;
	  digitalWrite(ledPin_g, ledState_g);
	}
      }
    } else {
      Serial.println("TURNING OFF LEDs");
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
      // Next Game State
      // delay(500);
      // digitalWrite(ledPin_r, HIGH);
      // digitalWrite(ledPin_y, HIGH);
      // digitalWrite(ledPin_g, HIGH);
      // delay(500);
      // digitalWrite(ledPin_r, LOW);
      // digitalWrite(ledPin_y, LOW);
      // digitalWrite(ledPin_g, LOW);
      // delay(500);
      counter = 0;	
      game_state = 1;
    }
  }
  // ======================================================================
  // Transmitting
  // ======================================================================
  if (game_state == 1) {
    if (sent_state == 0) {
      Serial.println("Now sending");
      if (!radio.write( &seq, sizeof(seq) )){
	Serial.println("Failed");
      }
      else {
	Serial.println("Success");
      }
      delay(2000);
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
	  if (state == 10){
	    state = 9;
	  }
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


