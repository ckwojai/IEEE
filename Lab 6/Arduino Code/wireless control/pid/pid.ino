#include <Wire.h>
#include "sensor_fusion.h"
#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
RF24 radio(9,10);
// Motor Variable
#define m1pin 5
#define m2pin 6
#define m3pin 9
#define m4pin 3
// Fusion Variable
int16_t x_a = 0;
int16_t y_a = 0;
int16_t z_a = 0;
int16_t x_g = 0;
int16_t y_g = 0;
int16_t z_g = 0;
int16_t x_g_old = 0;
int16_t y_g_old = 0;
int16_t z_g_old = 0;
int16_t bias_x_a = 0; 
int16_t bias_y_a = 0;
int16_t bias_z_a = 0;
int16_t bias_x_g = 0; 
int16_t bias_y_g = 0;
int16_t bias_z_g = 0;
int16_t count = 0;
unsigned long previousMillis = 0;
float alpha = 0.33;
vector acc_vec;
vector gyr_vec;
vector fil_vec;
apdata mdata;

void setup() {
  Serial.begin(9600);
  sensorSetup(50);
  radioSetup();
}

void loop() {
  getAngle(&mdata);
  radio.write(&mdata, sizeof(mdata));
  controlSpeed(&mdata,10,0,0,0);
  printData(&mdata);
  
  getAngle(&mdata);
  radio.write(&mdata, sizeof(mdata));  
  controlSpeed(&mdata,0,10,0,0);
  printData(&mdata);
  
  getAngle(&mdata);
  radio.write(&mdata, sizeof(mdata));
  controlSpeed(&mdata,0,0,10,0);
  printData(&mdata);
  
  getAngle(&mdata);
  radio.write(&mdata, sizeof(mdata));
  controlSpeed(&mdata,0,0,0,10);
  printData(&mdata);  
}
void printData(const struct apdata* d) {
  Serial.print(d->roll);
  Serial.print("  ");
  Serial.print(d->pitch);
  Serial.print("  ");  
  Serial.print(d->m1sp);
  Serial.print("  ");  
  Serial.print(d->m2sp);
  Serial.print("  ");  
  Serial.print(d->m3sp);
  Serial.print("  ");  
  Serial.println(d->m4sp);
}
void radioSetup() {
  radio.begin();
  radio.setChannel(16);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, 0xE7E7E7E7E7);
  radio.openWritingPipe(0xC2C2C2C2C2);
  radio.setCRCLength(RF24_CRC_16);
  printf_begin();
  radio.printDetails();  
}
void controlSpeed(struct apdata* sp, int m1sp, int m2sp, int m3sp, int m4sp) {
  sp->m1sp = m1sp;
  sp->m2sp = m2sp;
  sp->m3sp = m3sp;
  sp->m4sp = m4sp;  
  analogWrite(m1pin, sp->m1sp);
  analogWrite(m2pin, sp->m2sp);
  analogWrite(m3pin, sp->m3sp);
  analogWrite(m4pin, sp->m4sp);
}
void getAngle(struct apdata* d) {
  uint8_t status = 0;
  uint8_t temp = 0;
  readReg(INT_STATUS, &status, 1);
  if (status & 0b00000001 == 1) { /* LSB is 1 --> data ready*/
    unsigned long currentMillis = millis();
    /* Old value */
    x_g_old = x_g;
    y_g_old = y_g;
    z_g_old = z_g;
    /* resetting values */
    x_a = 0;
    y_a = 0;
    z_a = 0;
    x_g = 0;
    y_g = 0;
    z_g = 0;
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
    x_a = x_a - bias_x_a;
    y_a = y_a - bias_y_a;
    z_a = z_a - bias_z_a;
    x_g = x_g - bias_x_g;
    y_g = y_g - bias_y_g;
    z_g = z_g - bias_z_g;
    /* Accelerometer */
    acc_vec.x = x_a;
    acc_vec.y = y_a;
    acc_vec.z = z_a;
    vector_normalize(&acc_vec, &acc_vec);  
    /* Gyroscope */
    quaternion gyr_qua;
    int16_t x_g_rate;
    int16_t y_g_rate;
    int16_t z_g_rate;
    x_g_rate = x_g / 16.4; //- x_g_old;
    y_g_rate = y_g /16.4; //- y_g_old;
    z_g_rate = z_g /16.4; //- z_g_old;
    vector gyr_rate;
    gyr_rate.x = x_g_rate;
    gyr_rate.y = y_g_rate;
    gyr_rate.z = z_g_rate;
    float mag = vector_normalize(&gyr_rate, &gyr_rate);
    float angle = -1*mag *(currentMillis - previousMillis)*0.0174533 * 0.001;
    quaternion_create(&gyr_rate, angle, &gyr_qua);
    quaternion_rotate(&gyr_vec, &gyr_qua, &gyr_vec);
    previousMillis = currentMillis;  
   /* Filtering */
   vector acc_vec_fil = acc_vec;
   vector gyr_vec_fil = gyr_vec;
   quaternion_rotate(&fil_vec, &gyr_qua, &gyr_vec_fil);
   vector_multiply(&acc_vec, alpha, &acc_vec_fil);
   vector_multiply(&gyr_vec_fil, 1.0-alpha, &gyr_vec_fil);
   vector_add(&gyr_vec_fil, &acc_vec_fil, &fil_vec);
   vector_normalize(&fil_vec, &fil_vec);
    /* Real Data */
    d->roll = vector_roll(&fil_vec);
    d->pitch = vector_pitch(&fil_vec);
  }
}

void sensorSetup(const unsigned int N) {
  Wire.begin();        // join i2c bus (address optional for master)
  uint8_t pwr = 0;
  uint8_t gyro = 0;
  uint8_t config = 0;
  int16_t x_acc_arr[N];
  int16_t y_acc_arr[N];
  int16_t z_acc_arr[N];
  int16_t x_gyr_arr[N];
  int16_t y_gyr_arr[N];
  int16_t z_gyr_arr[N];  
  readReg(PWR_MGMT_1, &pwr, 1);
  readReg(GYRO_CONFIG, &gyro, 1);
  readReg(CONFIG, &config, 1);
  pwr = pwr & 0b10111111;
  gyro = gyro | 0b00011000;
  config = config & 0b11111000;
  writeReg(PWR_MGMT_1, &pwr, 1);
  writeReg(GYRO_CONFIG, &gyro, 1);
  writeReg(CONFIG, &config, 1);
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
      x_a = 0;
      y_a = 0;
      z_a = 0;
      x_g = 0;
      y_g = 0;
      z_g = 0;
      count++;
    }
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
  bias_x_a = x_avg_a / N;
  bias_y_a = y_avg_a / N;
  bias_z_a = z_avg_a / N - 16384;
  bias_x_g = x_avg_g / N;
  bias_y_g = y_avg_g / N;
  bias_z_g = z_avg_g / N;
  gyr_vec.x = 0;
  gyr_vec.y = 0;
  gyr_vec.z = 1;
  fil_vec.x = 0;
  fil_vec.y = 0;
  fil_vec.z = 1;
}
