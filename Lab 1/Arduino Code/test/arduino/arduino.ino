#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

RF24 radio(9,10);
void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(20);
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(1, 0xC2C2C2C2C2);
  radio.openWritingPipe(0xE7E7E7E7E7);
  radio.setCRCLength(RF24_CRC_16);
  radio.setRetries(15,15);
  printf_begin();
  delay(1000);
  radio.printDetails();
}

void loop() {
  // put your main code here, to run repeatedly:
  radio.startListening();
  Serial.print("Begin Listening...\n");
   unsigned long got_time;
    if( radio.available()){                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &got_time, sizeof(got_time) );             // Get the payload
      }
      radio.stopListening();                                        // First, stop listening so we can talk 
      Serial.println(got_time);
    } else {
      Serial.println("Failed");
    }
   delay(2000); 
}

