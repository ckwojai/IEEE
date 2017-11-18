#include <Wire.h>
#include <sensor_fusion.h>
const unsigned int N = 100;
int x_acc_arr[N];
int y_acc_arr[N];
int z_acc_arr[N];
int x_gyr_arr[N];
int y_gyr_arr[N];
int z_gyr_arr[N];
int x_a = 0;
int y_a = 0;
int z_a = 0;
int x_g = 0;
int y_g = 0;
int z_g = 0;
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
  /* calculate bias */
  while (count < N) {
    uint8_t status = 0;
    uint8_t temp = 0;
    readReg(INT_STATUS, &status, 1);
    if (status & 0b00000001 == 1) { /* LSB is 1 --> data ready*/
      /* getting ACCEL_X */
      readReg(ACCEL_XOUT_1, &temp, 1); /* read the first 8 bit */
      x_a = (x_a | temp) << 8;
      readReg(ACCEL_XOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
      x_a = x_a | temp;
      x_acc_arr[count] = x_a;
      temp = 0;
      /* getting ACCEL_Y */
      readReg(ACCEL_YOUT_1, &temp, 1); /* read the first 8 bit */
      y_a = (y_a | temp) << 8;
      readReg(ACCEL_YOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
      y_a = y_a | temp;
      y_acc_arr[count] = y_a;
      temp = 0;
      /* getting ACCEL_Z */
      readReg(ACCEL_ZOUT_1, &temp, 1); /* read the first 8 bit */
      z_a = (z_a | temp) << 8;
      readReg(ACCEL_ZOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
      z_a = z_a | temp;
      z_acc_arr[count] = z_a;
      temp = 0;
      /* getting GYRO_X */
      readReg(GYRO_XOUT_1, &temp, 1); /* read the first 8 bit */
      x_g = (x_g | temp) << 8;
      readReg(GYRO_XOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
      x_g = x_g | temp;
      x_gyr_arr[count] = x_g;
      temp = 0;
      /* getting GYRO_Y */
      readReg(GYRO_YOUT_1, &temp, 1); /* read the first 8 bit */
      y_g = (y_g | temp) << 8;
      readReg(GYRO_YOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
      y_g = y_g | temp;
      y_gyr_arr[count] = y_g;
      temp = 0;
      /* getting GYRO_Z */
      readReg(GYRO_ZOUT_1, &temp, 1); /* read the first 8 bit */
      z_g = (z_g | temp) << 8;
      readReg(GYRO_ZOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
      z_g = z_g | temp;
      z_gyr_arr[count] = z_g;
      temp = 0;
      /* INITIALIZATION */
      count++;
    }
    long int x_avg_a = 0;
    long int y_avg_a = 0;
    long int z_avg_a = 0;
    long int x_avg_g = 0;
    long int y_avg_g = 0;
    long int z_avg_g = 0;
    for (int i = 0; i < N; i++) {
      x_avg_a += x_acc_arr[i];
      y_avg_a += y_acc_arr[i];
      z_avg_a += z_acc_arr[i];
      x_avg_g += x_gyr_arr[i];
      y_avg_g += y_gyr_arr[i];
      z_avg_g += z_gyr_arr[i];
    }
    bias_x_a = 0 - x_avg_a / N;
    bias_y_a = 0 - y_avg_a / N;
    bias_z_a = 16384 - z_avg_a / N;
    bias_x_g = 0 - x_avg_g / N;
    bias_y_g = 0 - y_avg_g / N;
    bias_z_g = 0 - z_avg_g / N;
  }
}  
void loop()
{
  uint8_t status = 0;
  uint8_t temp = 0;
  readReg(INT_STATUS, &status, 1);
  if (status & 0b00000001 == 1) { /* LSB is 1 --> data ready*/
    /* getting ACCEL_X */
    readReg(ACCEL_XOUT_1, &temp, 1); /* read the first 8 bit */
    x_a = (x_a | temp) << 8;
    readReg(ACCEL_XOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
    x_a = x_a | temp;
    temp = 0;
    /* getting ACCEL_Y */
    readReg(ACCEL_YOUT_1, &temp, 1); /* read the first 8 bit */
    y_a = (y_a | temp) << 8;
    readReg(ACCEL_YOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
    y_a = y_a | temp;
    temp = 0;
    /* getting ACCEL_Z */
    readReg(ACCEL_ZOUT_1, &temp, 1); /* read the first 8 bit */
    z_a = (z_a | temp) << 8;
    readReg(ACCEL_ZOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
    z_a = z_a | temp;
    temp = 0;
    /* getting GYRO_X */
    readReg(GYRO_XOUT_1, &temp, 1); /* read the first 8 bit */
    x_g = (x_g | temp) << 8;
    readReg(GYRO_XOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
    x_g = x_g | temp;
    temp = 0;
    /* getting GYRO_Y */
    readReg(GYRO_YOUT_1, &temp, 1); /* read the first 8 bit */
    y_g = (y_g | temp) << 8;
    readReg(GYRO_YOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
    y_g = y_g | temp;
    temp = 0;
    /* getting GYRO_Z */
    readReg(GYRO_ZOUT_1, &temp, 1); /* read the first 8 bit */
    z_g = (z_g | temp) << 8;
    readReg(GYRO_ZOUT_0, &temp, 1); /* read the second 8 bit and combine to become a 16 bit */
    z_g = z_g | temp;
    temp = 0;
    /* Account for bias */
    /* x_a = x_a + bias_x_a; */
    /* y_a = y_a + bias_y_a; */
    /* z_a = z_a + bias_z_a; */
    /* x_g = x_g + bias_x_g; */
    /* y_g = y_g + bias_y_g; */
    /* z_g = z_g + bias_z_g; */
    /* Bias */
    /* Serial.print(bias_x_a); */
    /* Serial.print(" "); */
    /* Serial.print(bias_y_a); */
    /* Serial.print(" "); */
    /* Serial.print(bias_z_a); */
    /* Serial.print(" "); */
    /* Serial.print(bias_x_g); */
    /* Serial.print(" "); */
    /* Serial.print(bias_y_g); */
    /* Serial.print(" "); */
    /* Serial.print(bias_z_g); */
    /* Serial.println(""); */
    /* Real Data */
    Serial.print(x_a);
    Serial.print(" ");
    Serial.print(y_a);
    Serial.print(" ");
    Serial.print(z_a);
    Serial.print(" ");
    Serial.print(x_g);
    Serial.print(" ");
    Serial.print(y_g);
    Serial.print(" ");
    Serial.print(z_g);
    Serial.println("");
  }

  delay(1000);
}

