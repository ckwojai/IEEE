#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
/* ================================================== */
/* Used on ARDUINO to test TEENSY */
/* ================================================== */
RF24 radio(9,10);
int role = 0;
void setup() {
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
  if (role == 1) {
  Serial.println("Now sending");
   unsigned long match = 0;
     if (!radio.write( &match, sizeof(match))){
       Serial.println("Failed");
     }   
     role = 0;
  }
  if ( Serial.available() | role == 0)
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
    }
  }
}
