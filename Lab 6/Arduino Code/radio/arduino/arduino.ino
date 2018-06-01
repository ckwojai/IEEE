#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
/* ================================================== */
/* Global Variables */
/* ================================================== */
const int  ledPin = 4;    // the pin that the pushbutton is attached to
unsigned long pattern[10];
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
  pinMode(ledPin, OUTPUT);
}
void loop() {
  radio.startListening();
    if( radio.available()){    
       int i = 0;                                     
       while (radio.available()) {            
        radio.read(&pattern[i],sizeof(unsigned long));                     
        i++;
       }        
       Serial.print("Message received: the pattern is: ");
       for (int i = 0; i < 10; i++) {
    Serial.print(pattern[i]);
  }
} 
}
