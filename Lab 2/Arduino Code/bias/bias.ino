#include <Wire.h>
#include <sensor_fusion.h>
bool check_bias = true;
int x_acc[100];
int y_acc[100];
int z_acc[100];
int x_gyr[100];
int y_gyr[100];
int z_gyr[100];
int count = 0;
void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop()
{
  if (check_bias) {
    while (count < 100) {
      char status = 0;
      int temp = 0;
      readReg(INT_STATUS, status, 1);
      if (status & 0b00000001 == 1) { /* LSB is 1 --> data ready*/
	/* getting ACCEL_X */
	readReg(ACCEL_XOUT_1, temp, 1); /* read the first 8 bit */
	x_acc << 8;
	readReg(ACCEL_XOUT_0, temp, 1); /* read the second 8 bit and combine to become a 16 bit */
	x_acc[count] = temp;
	temp = 0;
	/* getting ACCEL_Y */
	readReg(ACCEL_YOUT_1, temp, 1); /* read the first 8 bit */
	y_acc << 8;
	readReg(ACCEL_YOUT_0, temp, 1); /* read the second 8 bit and combine to become a 16 bit */
	y_acc[count] = temp;
	temp = 0;
	/* getting ACCEL_Z */
	readReg(ACCEL_ZOUT_1, temp, 1); /* read the first 8 bit */
	z_acc << 8;
	readReg(ACCEL_ZOUT_0, temp, 1); /* read the second 8 bit and combine to become a 16 bit */
	z_acc[count] = temp;
	temp = 0;
	/* getting GYRO_X */
	readReg(GYRO_XOUT_1, temp, 1); /* read the first 8 bit */
	x_gyr << 8;
	readReg(GYRO_XOUT_0, temp, 1); /* read the second 8 bit and combine to become a 16 bit */
	x_gyr[count] = temp;
	temp = 0;
	/* getting GYRO_Y */
	readReg(GYRO_YOUT_1, temp, 1); /* read the first 8 bit */
	y_gyr << 8;
	readReg(GYRO_YOUT_0, temp, 1); /* read the second 8 bit and combine to become a 16 bit */
	y_gyr[count] = temp;
	temp = 0;
	/* getting GYRO_Z */
	readReg(GYRO_ZOUT_1, temp, 1); /* read the first 8 bit */
	z_gyr << 8;
	readReg(GYRO_ZOUT_0, temp, 1); /* read the second 8 bit and combine to become a 16 bit */
	z_gyr[count] = temp;
	temp = 0;
	/* INITIALIZATION */
	count++;
      }
    }
    check_bias = false;
  } else {
    Serial.print(x_gyr);
    Serial.print(y_gyr);
    Serial.print(z_gyr);
    Serial.print(x_acc);
    Serial.print(y_acc);
    Serial.print(z_acc);
  }
}