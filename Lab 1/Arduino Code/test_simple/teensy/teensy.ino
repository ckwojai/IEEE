b#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

RF24 radio(9,10);
void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(20);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, 0xE7E7E7E7E7);
  radio.openWritingPipe(0xC2C2C2C2C2);
  radio.setCRCLength(RF24_CRC_16);
  radio.setRetries(15,15);  
  printf_begin();
  delay(1000);
  radio.printDetails();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Now sending");
   unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
     if (!radio.write( &start_time, sizeof(start_time) )){
       Serial.println("Failed");
     }   
  delay(2000);
}
