
RF24::RF24( uint8_t 9, uint8_t 10)

void setup() {
  radio.begin()
  // put your setup code here, to run once:
  void RF24::setChannel(uint8_t 16)
  void RF24::setPALevel(uint8_t RF24_PA_MIN)
  //int length = str.length() - 1;
  bool key = true;
  int end = 1;
  
  string str;
  for (int i = 0; i < 8; i++){
  int j  = rand() % 2;
  if (j == 0)
    str.append("R");
  if (j == 1)
    str.append("Y");
  if (j == 2)  
    str.append("G");  
  } 
}

void loop() {
  // put your main code here, to run repeatedly:
 
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

