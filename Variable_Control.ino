
#define PWM 3
const int UPPERBOUND  =  100;
const int ZERO = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < UPPERBOUND; i++)
  {
    analogWrite(PWM, i);
  }
  delay(50);
  for (int i = 100; i > 0; i--) {
    analogWrite(PWM, i);
  }
  delay(50);
}
