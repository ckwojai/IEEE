#include <printf.h>
#include <SPI.h>
#include "RF24.h"

//1 = transmitting
//0 = receiving
bool role = 1;


/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/

void setup() {
  Serial.begin(9600);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  radio.begin();
  radio.setChannel(16); 
  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(0xE7E7E7E7E7);
    radio.openReadingPipe(1,0xC2C2C2C2C2);
  }else{
    radio.openWritingPipe(0xC2C2C2C2C2);
    radio.openReadingPipe(1,0xE7E7E7E7E7);
  }
  
  // Start the radio listening for data
  printf_begin();
  radio.printDetails();
  radio.startListening();
}

  //radio.begin()
  // put your setup code here, to run once:
  //void RF24::setChannel(uint8_t 16)
  //void RF24::setPALevel(uint8_t RF24_PA_MIN)


void loop() {
  // put your main code here, to run repeatedly:

//------------------HERE WE ARE ROLE 1
if (role == 1){
   //we will be transmitting so we need to stop listening
   radio.stopListening();
   Serial.print(F("Now sending"));

  unsigned long msg[10] ={0,1,2};
  //int msglen = 3;
  if (!radio.write(&msg, sizeof(msg))){
     Serial.println(F("failed"));
  }w

  radio.startListening();	//Continue Listening

  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
  while ( ! radio.available() ){                             // While nothing is received
  	if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
	   timeout = true;
           break;
	   }
  }


    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long end_time = micros();
        
        // Spew it
        Serial.print(F("Sent "));
        //Serial.print(start_time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(end_time-start_time);
        Serial.println(F(" microseconds"));
    }

    // Try again 1s later
    delay(1000);
  }
}
//--------------------END OF TRANNSMITTING



  string msg = "";
  //If key then send 
  if (key){
     msg = "012";
     
  }
  //otherwise wait
  else{
	
  }
  
  //contains the sequence that we will send 
  //string msg = "";
  int msglen = msg.length();

  //first sequence of length 1
  int j  = rand() % 3;
  if (j == 0)
    msg.append("R");
  else if (j == 1)
   msg.append("Y");
  else if (j == 2)  
    msg.append("G");
  
if (key == true)
for (int i = 0; i < end ; i++){
  char color = str[i];
  color(s);
}
end++;
key = false;

if (key == false)

}

bool color(char s)
{
  if (s == 'G')
    // send power to the wire that powers G
  if (s == 'R')
    // send power to the wire that powers R
  if (s == 'Y')
    // send power to the wire that powers Y
  delay(500);
}
}
