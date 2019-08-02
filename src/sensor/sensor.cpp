#include "sensor.h"
#include <Wire.h>

//makes an array to store samples
sample record[historyLen];
int8_t counter = 0;


//Turns on the MPU and wets the sample history.
void mpuSetup () {
#ifdef MPU6050
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
  // set to 8G max reading
  Wire.write(0b00010000);
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_addr);
  // Gyro config register
  Wire.write(0x1B);
  // set to 500 deg/sec max
  Wire.write(0b00001000);
  Wire.endTransmission(true);

#elif defined BNO055
  Wire.begin();

  //config mode
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3D);
  Wire.write(0b00000000);
  Wire.endTransmission(true);

  //Set acc units to milliGravs
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.write(0b00000001);
  Wire.endTransmission(true);

  //accelerometer set to 8G max
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x08);
  Wire.write(0b00000010);
  Wire.endTransmission(true);

  //NDOF mode
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3D);
  Wire.write(0b00001100);
  Wire.endTransmission(true);
#endif

  //Fill up the "record" vector.
  for (int i=0; i <= historyLen; i++) {
    switchSample();
  }

};

// Updates a sample when pointed to.
void sample::read () {

#ifdef MPU6050
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

#elif defined BNO055
  Wire.beginTransmission(MPU_addr);
  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.write(0x1A);
  // holds the I2C bus open
  Wire.endTransmission(false);
  // request a total of 14 registers
  Wire.requestFrom(MPU_addr,6);

  this->heading = Wire.read() | Wire.read()<<8;
  this->roll = Wire.read() | Wire.read()<<8;
  this->pitch = Wire.read() | Wire.read()<<8;

  //this->heading = Wire.read() | (Wire.read()<<8);
  //this->roll = Wire.read()|Wire.read()<<8;
  //this->pitch = Wire.read()|Wire.read()<<8;

#endif
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

lowPass lowPas;
lowPass fin;
highPass highPass;


double complementary (sample* data, float fc) {

  double a = lowPas.filter(atan2(data->acclX, data->acclZ)*(180/3.13),fc,dt);
  double g = highPass.filter(data->gyroY * (-500.0 / 32767), fc, dt);
  double post = g+a;
  double out = fin.filter(post, 0.5, dt);


  /*
  double alpha = 1 / ((2*3.14) * fc * dt + 1);
  double g = data->gyroY * (-500.0 / 32767);

  double gfilt = alpha * fHist + (1-alpha)*(g - gHist);
  gHist = g;
  fHist = gfilt;

  alpha = alpha * (2*3.14) * fc * dt;
  double acc = 90.0 - (atan2(data->acclZ , data->acclX) * (180.0/3.14));;
  double afilt = alpha*acc + (1-alpha)*aHist;
  aHist = afilt;

  Serial.print(afilt);Serial.print(",");
  Serial.print(gfilt);Serial.print(",");
  */
  return out;
};
