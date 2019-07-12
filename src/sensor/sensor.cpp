#include "sensor.h"
#include <Wire.h>

//makes an array to store samples
sample record[historyLen];
int8_t counter = 0;

float alpha = 0.06;
float angle = 0;
unsigned long time = 0;


//Turns on the MPU and wets the sample history.
void mpuSetup () {
  // Setup Gyro
  pinMode(62, OUTPUT);
  pinMode(63, OUTPUT);
  digitalWrite(62, HIGH);
  digitalWrite(63, HIGH);

  // Setup MPU
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  // PWR_MGMT_1 register
  Wire.write(0x6B);
  // set to zero (wakes up the MPU-6050)
  Wire.write(0);
  Wire.endTransmission(true);


  Wire.beginTransmission(MPU_addr);
  // Accelerometer config register
  Wire.write(0x1C);
  // set to most course reading
  Wire.write(0b00011000);
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_addr);
  // Gyro config register
  Wire.write(0x1B);
  // set to most course reading
  Wire.write(0b00011000);
  Wire.endTransmission(true);

  //Fill up the "record" vector.
  for (int i=0; i < 5; i++) {
    switchSample();
  }

};

// Updates a sample when pointed to.
void sample::read () {
  // Wake up the sensor
  Wire.beginTransmission(MPU_addr);
  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.write(0x3B);
  // holds the I2C bus open
  Wire.endTransmission(false);
  // request a total of 14 registers
  Wire.requestFrom(MPU_addr,14);


  //READS DATA TO MEMORY
  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  this->acclX = Wire.read()<<8|Wire.read();
  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  this->acclY = Wire.read()<<8|Wire.read();
  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  this->acclZ = Wire.read()<<8|Wire.read();

  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  this->temp=Wire.read()<<8|Wire.read();

  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  this->gyroX = Wire.read()<<8|Wire.read();
  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  this->gyroY = Wire.read()<<8|Wire.read();
  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  this->gyroZ = Wire.read()<<8|Wire.read();
};

// Moves to the next sample and updates it.
void switchSample () {
  //moves to the next location in "record"
  counter++;
  if (counter >= historyLen) {
    counter = 0;
  }
  //updates the new locations in record.
  record[counter].sample::read() ;
}

// Updates a sample and returns a pointer to it
sample* getSample () {
  switchSample();
  return &record[counter];
};

double complementary (sample* data) {
  double gyro = -1 * ((millis()-time)/1000.)*(22 + data->gyroY) / (10500000000.);

  double rads = 95.0 - (atan2(data->acclZ , data->acclX) * (180.0/3.14));

  time = micros();

  angle = (1-alpha)*(angle + gyro) + (alpha)*(rads);

  return angle;
};
