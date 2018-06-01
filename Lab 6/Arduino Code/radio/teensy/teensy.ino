#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
long unsigned seq[10] = {0,1,2,3,4,5,6,7,8,9};
const int  buttonPin = 3;
int buttonState = 0;
RF24 radio(9,10);
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  radio.begin();
  radio.setChannel(16);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, 0xE7E7E7E7E7);
  radio.openWritingPipe(0xC2C2C2C2C2);
  radio.setCRCLength(RF24_CRC_16);
  printf_begin();
  radio.printDetails();
}

void loop() {
  /* ====================================================================== */
  /* Transmitting */
  /* ====================================================================== */
    buttonState = digitalRead(buttonPin);
    if (1) {
      Serial.println("Now sending");
      radio.stopListening();
      for (int i=0; i<10; i++) {
         while (!radio.write(&seq[i], sizeof(seq)));
      }
      Serial.println("Done sending");
    }                                      
    delay(500);
}



