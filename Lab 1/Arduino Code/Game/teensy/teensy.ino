#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
const int SEQLEN = 10; /* 0 = doesn't match; 1 = match; */
int game_state = 0; /* 0 = displaying; 1 = transmit/waiting  */
int sent_state = 0; /* 0 = wait to be sent; 1 = sent and wait for response */
int state = 1; /* Keeping track of the LED game state we are in */
int counter = 0; /* index of array to keep track of the current LED state */
unsigned long match; /* 0 = doesn't match; 1 = match; */

RF24 radio(9,10);
long unsigned seq[SEQLEN] = {0,1,2,0,1,2,0,1,2,0};  /* seq = r y g r y g r y g r */
const int ledPin_r = 3;
const int ledPin_y = 4;
const int ledPin_g = 5;
int ledState_r = LOW;
int ledState_y = LOW;
int ledState_g = LOW;
long interval = 2500;		/* Interval Between LED flashes */
unsigned long previousMillis = 0; /* Keeping track of time the previous LED lights up */

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(16);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, 0xE7E7E7E7E7);
  radio.openWritingPipe(0xC2C2C2C2C2);
  radio.setCRCLength(RF24_CRC_16);
  printf_begin();
  radio.printDetails();
  /* Randomization code */
  srand((unsigned)analogRead(0));
  for (int i = 0; i < SEQLEN; i++) {
    seq[i] = rand() % 3;
  }
  for (int i = 0; i < SEQLEN; i++) {
    Serial.print(seq[i]);
  }
  Serial.println("");
  /* Output pins */
  pinMode(ledPin_r, OUTPUT);
  pinMode(ledPin_y, OUTPUT);
  pinMode(ledPin_g, OUTPUT);
  digitalWrite(ledPin_r, HIGH);
  digitalWrite(ledPin_y, HIGH);
  digitalWrite(ledPin_g, HIGH);
  delay(1000);
  digitalWrite(ledPin_r, LOW);
  digitalWrite(ledPin_y, LOW);
  digitalWrite(ledPin_g, LOW);
  delay(1000);
}

void loop() {
  if (game_state == 0) { /* Displaying the LEDs, in sequence */
    unsigned long currentMillis = millis();
    /* ================================================== */
    /* Turning off LEDs */
    /* ================================================== */
    if (counter < state) { /* LED haven't reached its maximum state, keep lighting */
      if(currentMillis - previousMillis > interval) { /* Keeping track of the time  */
	if (counter != 0) { /* Don't turn LEDs off on the first run */
	  if (seq[counter-1] == 0) {	// Turning OFF the red
	    ledState_r = LOW;
	    digitalWrite(ledPin_r, ledState_r);
	    Serial.println("RED OFF");
	  } else if (seq[counter-1] == 1) { // Turning OFF the yellow
	    ledState_y = LOW;
	    digitalWrite(ledPin_y, ledState_y);
	    Serial.println("YELLOW OFF");
	  } else if (seq[counter-1] == 2) { // Turning OFF the green
	    ledState_g = LOW;
	    digitalWrite(ledPin_g, ledState_g);
	    Serial.println("GREEN OFF");
	  }
	}
	delay(1000);		/* Delay between previous light turning off and new light turning on  */
	/* save the last time you blinked the LED  */
	/* ================================================== */
	/* Turning on LEDs */
	/* ================================================== */
	previousMillis = currentMillis;   
	if (seq[counter] == 0) {	// Light up the red
	  counter++;
	  ledState_r = HIGH;
	  digitalWrite(ledPin_r, ledState_r);
	  Serial.println("RED ON");
	} else if (seq[counter] == 1) { // Light up the yellow
	  counter++;
	  ledState_y = HIGH;
	  digitalWrite(ledPin_y, ledState_y);
	  Serial.println("Yellow ON");
	} else if (seq[counter] == 2) { // Light up the green
	  counter++;
	  ledState_g = HIGH;
	  digitalWrite(ledPin_g, ledState_g);
	  Serial.println("Green ON");
	}
      }
    } else {
      delay(1000); /* Dealy before turning off the last LED */
      if (seq[counter-1] == 0) {	/* Turn OFF the red */
	ledState_r = LOW;
	digitalWrite(ledPin_r, ledState_r);
	Serial.println("RED OFF");
      } else if (seq[counter-1] == 1) { /* Turn OFF the yellow */
	ledState_y = LOW;
	digitalWrite(ledPin_y, ledState_y);
	Serial.println("YELLOW OFF");
	} else if (seq[counter-1] == 2) { /* Turn OFF the green */
	ledState_g = LOW;
	digitalWrite(ledPin_g, ledState_g);
	Serial.println("GREEN OFF");
      }
      /* Resetting */
      counter = 0;	
      game_state = 1;
      sent_state = 0;
    }
  }
  /* ====================================================================== */
  /* Transmitting */
  /* ====================================================================== */
  if (game_state == 1) {
    if (sent_state == 0) {	/* Message not yet sent, attempting to send */
      Serial.println("Now sending");
      radio.stopListening();
      while (!radio.write( &seq[state-1], sizeof(seq))) {
      }
      /* int count = 0; */
      /* while (count < 100) { */
      /* 	radio.write(&seq[state-1], sizeof(seq)); */
      /* 	count ++; */
      /* 	Serial.println(count); */
      /* } */
    Serial.println("Success");
    sent_state = 1; /* Messaged Sent, Switch to listeing state */
    Serial.print("Begin Listening...\n");
    } else if (sent_state == 1) {
      radio.startListening();
      if( radio.available()){                                         
	game_state = 0;
	while (radio.available()) {                                   
	  radio.read( &match, sizeof(match));             
	}
	radio.stopListening();                                        
	Serial.println(match);
	if (match == 0) {
	  ledState_r = HIGH;
	  /* Doesn't Match, Red LED lights up and game restart*/
	  digitalWrite(ledPin_r, ledState_r);
	  delay(2000);
	  ledState_r = LOW;
	  digitalWrite(ledPin_r, ledState_r);
	  delay(2000);
	  Serial.println("Not Matched");
	  /* Restart game */
	    for (int i = 0; i < SEQLEN; i++) {
	      seq[i] = rand() % 3;
	    }
	  counter = 0;
	  state = 1;
	} else if (match == 1) {
	  /* Does Match: Green LED lights up and game continues */
	  ledState_g = HIGH;
	  digitalWrite(ledPin_g, ledState_g);
	  delay(1000);
	  ledState_g = LOW;
	  digitalWrite(ledPin_g, ledState_g);
	  delay(1000);
	  Serial.println("Matched");
	  digitalWrite(ledPin_r, HIGH);
	  digitalWrite(ledPin_y, HIGH);
	  digitalWrite(ledPin_g, HIGH);
	  delay(1000);
	  digitalWrite(ledPin_r, LOW);
	  digitalWrite(ledPin_y, LOW);
	  digitalWrite(ledPin_g, LOW);
	  delay(1000);
	  counter = 0;
	  state +=1;
	}
      }
    }
  }
}



