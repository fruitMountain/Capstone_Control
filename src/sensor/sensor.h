#pragma once
#include <Arduino.h>
#include "filter.h"

/*
  Here we will define which sensor we are using.
  Current options include:
  + MPU6050
  + BNO055
 */
#define BNO055

// Holds Gyro and Accelerometer data
class sample {
 public:
  int acclX = 0, acclY = 0, acclZ = 0;
  int gyroX = 0, gyroY = 0, gyroZ = 0;
  int temp = 0;

  int roll = 0, pitch = 0, heading = 0;

  // Updates sensor data
  void read ();
};

// Some variables for filtering ***
extern float angle;
extern float dt;
extern double aHist;
extern double gHist;
extern double fHist;

// Find function definitions in sensor.cpp or README
void mpuSetup ();
void switchSample ();
sample* getSample ();
double complementary (sample* data, float fc);

//Makes an array to store samples
#define historyLen 5
extern sample record[historyLen];
extern int8_t counter;

// I2C address of the MPU based on whats connected
#ifdef MPU6050
#define MPU_addr 0x68
#elif defined BNO055
#define MPU_addr 0x28
#endif
