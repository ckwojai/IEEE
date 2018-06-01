#define motor1 3
#define motor2 5
#define motor3 6
#define motor4 9
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(motor1, 0);
   analogWrite(motor2, 1);
    analogWrite(motor3, 2);
     analogWrite(motor4, 3);
}
