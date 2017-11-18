#include <Wire.h>
#include <sensor_fusion.h>
char a[10];
char b[10];
char c[10];
void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  writeReg(PWR_MGMT_1, 0b11111111, 1);
  writeReg(GYRO_CONFIG, 0b11111111, 1);
  writeReg(PWR_MGMT_1, 0b11111111, 1);
  readReg(PWR_MGMT_1, a, 1);
  readReg(GYRO_CONFIG, b, 1);
  readReg(CONFIG, c, 1);
  /* writeReg(PWR_MGMT_1, a & 0b10111111, 1);	       /\* PWR_MGMT_1: seoncd bit to 0 *\/ */
  /* writeReg(GYRO_CONFIG, b | 0b00011000, 1);	       /\* PWR_MGMT_1 4 and 5 bit to 1*\/ */
  /* writeReg(CONFIG, c & 0b11111000, 1);	       /\* PWR_MGMT_1 4 and 5 bit to 1*\/ */
  /* readReg(PWR_MGMT_1, a, 1); */
  /* readReg(GYRO_CONFIG, b, 1); */
  /* readReg(CONFIG, c, 1); */
  Serial.begin(9600);  // start serial for output
}

void loop()
{
  /* readReg(PWR_MGMT_1, a, 1); */
  /* readReg(GYRO_CONFIG, b, 1); */
  /* readReg(CONFIG, c, 1); */
  Serial.println(a[0]);
  Serial.println(b[0]);
  Serial.println(c[0]);
  Serial.println("idk");
  delay(1000);
  /* while(Wire.available())    // slave may send less than requested */
  /* {  */
  /*   char c = Wire.read();    // receive a byte as character */
  /*   Serial.print(c);         // print the character */
  /* } */

  /* delay(500); */
}