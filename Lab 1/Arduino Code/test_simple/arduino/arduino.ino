#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

RF24 radio(9,10);
void setup() {
  while(!Serial);
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(16);
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(1, 0xC2C2C2C2C2);
  radio.openWritingPipe(0xE7E7E7E7E7);
  radio.setCRCLength(RF24_CRC_16);
  printf_begin();
  radio.printDetails();
}
void loop() {
  radio.startListening();
  Serial.print("Begin Listening...\n");
   unsigned long got_time;
    if( radio.available()){
      while (radio.available()) {
	radio.read( &got_time, sizeof(got_time));
      }
      radio.stopListening();
      Serial.println(got_time);
    } else {
      Serial.println("Failed");
    }
   delay(1000);
}
