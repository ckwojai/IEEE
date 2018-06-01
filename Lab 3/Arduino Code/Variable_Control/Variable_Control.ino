
#define PWM 5
const int UPPERBOUND  =  255;
const int ZERO = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
    analogWrite(PWM, 1);
//  for (int i = 0; i < UPPERBOUND; i+=10)
//  {
//    analogWrite(PWM, i);
//     delay(100);
//  }
// 
//  for (int i = UPPERBOUND; i > 0; i-=10) {
//    analogWrite(PWM, i);
//    delay(100);
//  }

}
