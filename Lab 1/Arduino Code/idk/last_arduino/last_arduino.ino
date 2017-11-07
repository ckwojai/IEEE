#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

RF24 radio(6,7);
void setup() {
  radio.begin();
  Serial.begin(9600);
  radio.setChannel(100);
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(2, 0xd3d3d3d3d3);
  radio.openWritingPipe(0xf8f8f8f8f8);
  radio.setCRCLength(RF24_CRC_16);
  radio.startListening();
  printf_begin();
  radio.printDetails();
}

void loop() {
  radio.stopListening();
  Serial.println("Now sending");
  unsigned long start_time = micros();
  if (!radio.write( &start_time, sizeof(start_time))){
    Serial.println("Failed");
  }   
  delay(2000);
}
