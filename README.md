# Quadcopter Controller

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [Quadcopter Controller](#quadcopter-controller)
    - [Project Information](#project-information)
    - [How to use this code](#how-to-use-this-code)
        - [Sensor.h](#sensorh)
            - [Keeping Track of Samples](#keeping-track-of-samples)
            - [The Sample Class](#the-sample-class)
            - [The switchSample Function](#the-switchsample-function)
            - [The getSample Function](#the-getsample-function)
            - [The Complementary function](#the-complementary-function)
        - [Motor.h](#motorh)
            - [The escSetup Function](#the-escsetup-function)
            - [The turnMotor Function](#the-turnmotor-function)
    - [Calibrating the Sensor](#calibrating-the-sensor)

<!-- markdown-toc end -->


## Project Information
This software was developed by a team of University of Idaho students working to create a multi-rotor drone for use in research, for fields such as control theory and environmental monitoring.

## How to use this code
Each header file has a few functions and classes to help get rotor-craft up and running. Here they are broken down by header file, each of which is centered on a particular part or function.

### Sensor.h
This file is used to interface with an MPU, and is already set up to interface with a MPU6050 or a BNO055. In "sensor.h" line 11 there is a preprocessor directive which allows you to choose which sensor to use.

The only function I would expect to be used in a main function is "getSample", which returns a pointer to the most recent sensor data in a sample object. These are handled with a few nested functions to perform the necessary operations to handle this format.

#### Keeping Track of Samples
There are three main parts to keeping track, internally, of what the sensor is reading.
1.  The "record" array
    This array contains "samples", objects to keep track of information about each time the sensor reads.
2.  historyLen
    This is a pre-processor #define term. It is equal to the length of the record array, and can be changed at the bottom of sensor.h
3.  The counter integer
    "counter", an int variable, is used to keep track of which term of counter was most recently updated.

#### The Sample Class
Each sample holds information from an accelerometer and a gyroscope in the X, Y, and Z axes, as well as the internal temperature of the MPU. The sample class also contains a member function, "read", that updates the sample it is pointed to. The read function is called with the syntax:

    object.sample::read(**;

#### The switchSample Function
The purpose of this function is the move to the next sample in the record array and to update its sensor reading. It accomplishes this by using the read function, which reads from registers in the sensor.

#### The getSample Function
This updates a sample and returns a pointer to it.

### Filter.h
housed within the sensor directory this header defines a number of filter objects you can use to clean unwanted signals from your sensor data.

#### The Complementary function
This essentially turns your sample into an angle. It happens to use a complementary filter, hence the name. This works by low passing the somewhat noisy accelerometer data, and high passing the gyroscope data to correct for drift, then adding them together.

### Motor.h
This is a header file to control the ESCs used in the Seesaw. There isn't much going on in here, but it makes it easy to switch ESCs if that were to come up, without searching other code for where the motors are interacted with.

The motors are interacted with as Servos, meaning that every 20ms the arduino sends a pwm signal between 1ms and 2ms in length. This is triggered with the writeMicroseconds member function in the servo class, passing it an integer between 1000 and 2000. Electric motors don't like to turn at really slowly, so there is a minimum speed of 1075, in this case a pre-processor directive.

#### The escSetup Function
This function was mostly written by Preston Stephens, as part of the 2018-19 team. It attaches the ESCs, then writes a high and low signal for five seconds each to calibrate the ESCs. Finally it ramps the motors up from a standstill to the idle speed.

#### The turnMotor Function
This is relatively simple. You pass it two integers between 1000 and 2000, then it turns the motors at those two speeds

## Calibrating the Sensor
The sensor's sensitivity can be changed in the mpuSetup function. The lines commented "config register" allow you to change the settings of sensors on the MPU-6050 by setting certain bits high or low. More information can be found in the MPU-6050 register map.
