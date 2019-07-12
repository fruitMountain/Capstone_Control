# Quadcopter Controller

<!-- toc -->

## Project Information
+ What?
  - A capstone design project at the University of Idaho
+ Who?
  - Seth Berryhill as part of the Multi-rotor Drone Control System Project
+ Why?
  - We are building research tools for future scientists here the UIdaho to experiment with unmanned aircraft.

## How to use this code
I wrote some functions to make life easier when your trying to rework this and control a new system or do something else fun, like implementing optimal control. Here they are listed by header file and defined.

### Sensor.h
This file is used to interface with an MPU, please write down which MPU you have written for so it's easier to keep track. The only function I would expect to be used in a main function is "getSample". The funny business here is that we are storing data in an array of "sample" objects. The other funny business is the Russian doll set of functions.

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

    object.sample::read();

#### The switchSample Function
This function moves the counter variable along to the next term in the record array, then updates that sample object using the read function. This is the second Russian doll, because the only use for the read function is to be called here, and the only use for this function is to be called by the getSample function.

#### The getSample Function
This updates a sample and returns a pointer to it.

#### The Complementary function
This essentially turns your sample into an angle. It happens to use a complementary filter, hence the name. This works by low passing the somewhat noisy accelerometer data, and high passing the gyroscope data to correct for drift, then adding them together.

### Motor.h
This is a header file to control the ESCs used in the Seesaw. There isn't much going on in here, but it makes it easy to switch ESCs if that were to come up, without searching other code for where the motors are interacted with.

The motors are interacted with as Servos, meaning that every 20ms the arduino sends a pwm signal between 1ms and 2ms in length. This is triggered with the writeMicroseconds member function in the servo class, passing it an integer between 1000 and 2000. Electric motors don't like to turn at really slowly, so there is a minimum speed of 1075, in this case a pre-processor directive.

#### The escSetup Function
This function was mostly written by Preston Stephens, as part of the 2018-19 team. It attaches the ESCs, then writes a high and low signal for five seconds each to calibrate the ESCs. Finally it ramps the motors up from a standstill to the idle speed.

#### The turnMotor Function
This is relatively simple. You pass it two integers between 1000 and 2000, then it turns the motors at those two speeds
