#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

RF24 radio(9,10);
int role = 0;
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
  radio.printDetails();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (role == 1) {
  Serial.println("Now sending");
  unsigned long match[10] = {0,1,2,0,1,2,0,1,2,0};                             // Take the time, and send it.  This will block until complete
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
