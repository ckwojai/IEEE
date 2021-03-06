#include <Arduino.h>
#include <Wire.h>
#include "sensor_fusion.h"

void apdata_add(struct apdata* a1, struct apdata* a2, struct apdata* result) {
  result->roll = a1->roll + a2->roll;
  result->pitch = a1->pitch + a2->pitch;  
  result->m1sp = a1->m1sp + a2->m1sp;
  result->m2sp = a1->m2sp + a2->m2sp;
  result->m3sp = a1->m3sp + a2->m3sp;
  result->m4sp = a1->m4sp + a2->m4sp;  
}
void apdata_minus(struct apdata* a1, struct apdata* a2, struct apdata* result) {
  result->roll = a1->roll - a2->roll;
  result->pitch = a1->pitch - a2->pitch;    
  result->m1sp = a1->m1sp - a2->m1sp;
  result->m2sp = a1->m2sp - a2->m2sp;
  result->m3sp = a1->m3sp - a2->m3sp;
  result->m4sp = a1->m4sp - a2->m4sp;  
}

void setData(struct apdata* d, float roll, float pitch, float m1sp, float m2sp, float m3sp, float m4sp) {
  d->roll = roll;
  d->pitch = pitch;
  d->m1sp = m1sp;
  d->m2sp = m2sp;
  d->m3sp = m3sp;
  d->m4sp = m4sp;  
}


void readReg(uint8_t reg, uint8_t *buf, size_t len)
{
    // TODO: Implement
  byte ADD_S = 0b1101000;	// AD0 pin is logic low; slave address 7 bit
  Wire.beginTransmission(ADD_S); // 7 bit plus 0 (write bit)
  Wire.write(reg);
  Wire.endTransmission(false);  
  Wire.requestFrom(ADD_S, len);	  // parameter 7 bit slave address, the last bit will be added automiatically 1 (read)
  for (int i = 0; i < len; i++) { // actual reading
    buf[i] = Wire.read();
  }
  Wire.endTransmission();    
}

void writeReg(uint8_t reg, uint8_t *buf, size_t len)
{
    // TODO: Implement
  byte ADD_S = 0b1101000;
  Wire.beginTransmission(ADD_S); // 0 write bit
  Wire.write(reg);
  for (int i = 0; i < len; i++) {
    Wire.write(buf[i]);
  }
  Wire.endTransmission();  
}

float vector_normalize(struct vector *raw, struct vector *unit)
{
  float mag = sqrt(pow(raw->x, 2) + pow(raw->y, 2) + pow(raw->z, 2));
  if(mag != 0) {
      unit->x = raw->x / mag;
      unit->y = raw->y / mag;
      unit->z = raw->z / mag;
  }
  return mag;
}

void vector_add(struct vector *v1, struct vector *v2, struct vector *result)
{
  result->x = v1->x + v2->x;
  result->y = v1->y + v2->y;
  result->z = v1->z + v2->z;
}

void vector_multiply(struct vector *v, float c, struct vector *result)
{
  result->x = v->x * c;
  result->y = v->y * c;
  result->z = v->z * c;
}


void quaternion_create(struct vector *v, float angle, struct quaternion *result)
{
  result->r = cos(angle/2);
  result->i = v->x * sin(angle/2);
  result->j = v->y * sin(angle/2);
  result->k = v->z * sin(angle/2);
}

void quaternion_rotate(struct vector *v, struct quaternion *q, struct vector *result)
{
  struct vector n;
  memcpy(&n, v, sizeof(n));
  result->x = n.x * (1 - 2*(pow(q->j, 2) + pow(q->k, 2))) + n.y * 2*(q->i * q->j - q->k * q->r) + n.z * 2*(q->i * q->k + q->j * q->r);
  result->y = n.x * 2*(q->i * q->j + q->k * q->r) + n.y * (1 - 2*(pow(q->i, 2) + pow(q->k, 2))) + n.z * 2*(q->j * q->k - q->i * q->r);
  result->z = n.x * 2*(q->i * q->k - q->j * q->r) + n.y * 2*(q->j * q->k + q->i * q->r) + n.z * (1 - 2*(pow(q->i, 2) + pow(q->j, 2)));
}

void quaternion_multiply(struct quaternion *q1, struct quaternion *q2, struct quaternion *result)
{
  result->r = q1->r * q2->r - q1->i * q2->i - q1->j * q2->j - q1->k * q2->k;
  result->i = q1->r * q2->i + q1->i * q2->r + q1->j * q2->k - q1->k * q2->j;
  result->j = q1->r * q2->j - q1->i * q2->k + q1->j * q2->r + q1->k * q2->i;
  result->k = q1->r * q2->k + q1->i * q2->j - q1->j * q2->i + q1->k * q2->r;
}

float vector_roll(struct vector *v)
{
  return atan2(v->x, sqrt(pow(v->z, 2) + pow(v->y, 2)));
}

float vector_pitch(struct vector *v)
{
  return -atan2(v->y, v->z);
}


