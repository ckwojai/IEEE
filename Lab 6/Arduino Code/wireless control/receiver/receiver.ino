#include <Wire.h>
#include "sensor_fusion.h"
#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
/* ================================================== */
/* Global Variables */
/* ================================================== */
RF24 radio(9,10);
void setup() {
  // initialize the button pin as a input:
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(16);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, 0xC2C2C2C2C2);
  radio.openWritingPipe(0xE7E7E7E7E7);
  radio.setCRCLength(RF24_CRC_16);
  printf_begin();
  radio.printDetails();
}
void loop() {
  radio.startListening();
  apdata idk;
  // Serial.println("Listening");
    if(radio.available()){
       while (radio.available()) {            
        radio.read(&idk,sizeof(idk));
       }
  Serial.print(idk.roll);
  Serial.print("   ");
  Serial.print(idk.pitch);
  Serial.print("   ");
  Serial.print(idk.m1sp);
  Serial.print("   ");
  Serial.print(idk.m2sp);
  Serial.print("   ");
  Serial.print(idk.m3sp);
  Serial.print("   ");
  Serial.println(idk.m4sp);
  }

}
