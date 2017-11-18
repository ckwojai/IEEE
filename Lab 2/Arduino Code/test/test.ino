#include <Wire.h>
#include <sensor_fusion.h>
uint8_t a;
uint8_t b;
uint8_t c;
void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  uint8_t pwr = 0;
  uint8_t gyro = 0;
  uint8_t config = 0;
  readReg(PWR_MGMT_1, &pwr, 1);
  readReg(GYRO_CONFIG, &gyro, 1);
  readReg(CONFIG, &config, 1);
  pwr = pwr & 0b10111111;
  gyro = gyro | 0b00011000;
  config = config & 0b11111000;
  writeReg(PWR_MGMT_1, &pwr, 1);
  writeReg(GYRO_CONFIG, &gyro, 1);
  writeReg(PWR_MGMT_1, &config, 1);
  readReg(PWR_MGMT_1, &a, 1);
  readReg(GYRO_CONFIG, &b, 1);
  readReg(CONFIG, &c, 1);
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
  Serial.println(a);
  Serial.println(b);
  Serial.println(c);
  Serial.println("idk");
  delay(1000);
  /* while(Wire.available())    // slave may send less than requested */
  /* {  */
  /*   char c = Wire.read();    // receive a byte as character */
  /*   Serial.print(c);         // print the character */
  /* } */

  /* delay(500); */
}