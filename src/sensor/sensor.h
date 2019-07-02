// GY-521

#pragma once
#include <Arduino.h>



// I2C address of the MPU-6050
const int MPU_addr=0x68;


// Holds Gyro and Accelerometer data
class sample {
 public:
  int acclX, acclY, acclZ;
  int gyroX, gyroY, gyroZ;
  int temp;

  // Updates sensor data
  void read ();
};


// Find function definitions in sensor.cpp or README
void mpuSetup();
void switchSample();
sample* getSample();

//Makes an array to store samples
#define historyLen 5
extern sample record[historyLen];
extern int8_t counter;
