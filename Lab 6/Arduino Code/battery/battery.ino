#include <Wire.h>
#include "sensor_fusion.h"
#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
RF24 radio(9,10);
#define ledpin 4
void setup() {
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
  radioSetup();
  checkVolt();
}
void loop() {
  checkVolt();
}
void checkVolt() {
  int rawReading = analogRead(A3);
  float voltage = rawReading * (3.7 / 1023.0);
  Serial.print("BATTERY LEVEL IS:    ");
  Serial.println(voltage);
  radio.write(&voltage, sizeof(float));
  if (voltage < 3.7 / 2) {
	digitalWrite(ledpin, HIGH);
  } else {
	digitalWrite(ledpin, LOW);
  }
}
void radioSetup() {
  radio.begin();
  radio.setChannel(16);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, 0xE7E7E7E7E7);
  radio.openWritingPipe(0xC2C2C2C2C2);
  radio.setCRCLength(RF24_CRC_16);
  printf_begin();
  Serial.println("Prining shits out!!");
  radio.printDetails();  
}
