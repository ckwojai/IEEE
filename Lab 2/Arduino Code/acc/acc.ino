#include <Wire.h>
#include <sensor_fusion.h>
const unsigned int N = 100;
bool check_bias = true;
bool calc_bias = true;
int x_acc[N];
int y_acc[N];
int z_acc[N];
int x_gyr[N];
int y_gyr[N];
int z_gyr[N];
int x_avg_a = 0;
int y_avg_a = 0;
int z_avg_a = 0;
int x_avg_g = 0;
int y_avg_g = 0;
int z_avg_g = 0;
int bias_x_a = 0; 
int bias_y_a = 0;
int bias_z_a = 0;
int bias_x_g = 0; 
int bias_y_g = 0;
int bias_z_g = 0;
int count = 0;
void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop()
{
  if (check_bias) {
    while (count < N) {
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
    if (calc_bias) {
      x_avg_a = 0;
      y_avg_a = 0;
      z_avg_a = 0;
      x_avg_g = 0;
      y_avg_g = 0;
      z_avg_g = 0;
      for (int i = 0; i < N; i++) {
	x_avg_a += x_acc[i];
	y_avg_a += y_acc[i];
	z_avg_a += z_acc[i];
	x_avg_g += x_gyr[i];
	y_avg_g += y_gyr[i];
	z_avg_g += z_gyr[i];
      }
      bias_x_a = 0 - x_avg_a / N;
      bias_y_a = 0 - y_avg_a / N;
      bias_z_a = 0 - z_apvg_a / N;
      bias_x_g = 0 - x_avg_g / N;
      bias_y_g = 0 - y_avg_g / N;
      bias_z_g = 0 - z_avg_g / N;
      calc_bias = false;
    } else {
      Serial.print(x_gyr);
      Serial.print(y_gyr);
      Serial.print(z_gyr);
      Serial.print(x_acc);
      Serial.print(y_acc);
      Serial.print(z_acc);
    }
  }
}
