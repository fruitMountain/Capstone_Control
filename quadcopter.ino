#include<Wire.h>
#include <Servo.h>//Using servo library to control ESC

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

Servo esc_0; //Creating a servo class with name as esc
Servo esc_1;

int base_pwm = 1175;

void setup()
{

  // Setup Gyro
  pinMode(62, OUTPUT);
  pinMode(63, OUTPUT);
  digitalWrite(62, HIGH);
  digitalWrite(63, HIGH);

  // Setup MPU
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1C);  // Accelerometer config register
  Wire.write(0b00011000);     // set to most course reading
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1B);  // Gyro config register
  Wire.write(0b00011000);     // set to most course reading
  Wire.endTransmission(true);

  Serial.begin(9600);

// Setup ESCs
   /*
   *  To Calibrate ESC:
   *  1. Disconnect Power supply
   *  2. Set pwm to max signal (2000 us)
   *  3. Reconnect Power Supply, then wait a couple seconds
   *  4. Set pwm to min signal (1000 us)
   *  5. Wait a couple of seconds, then set signal as desired
   *
   *  With this code:
   *  1. Disconnect the power
   *  2. Hit reset on Arduino
   *  3. Reconnect the power
   */

  esc_0.attach(9);
  esc_1.attach(10);
  esc_0.writeMicroseconds(2000); // Send Stop Command
  esc_1.writeMicroseconds(2000);
  delay(5000);
  esc_0.writeMicroseconds(1000); // Send Start Command
  esc_1.writeMicroseconds(1000);
  delay(5000);

  for(int i = 1000; i < base_pwm; i++){
    esc_0.writeMicroseconds(i);
    esc_1.writeMicroseconds(i);
    delay(100);
  }

}

void loop() {

// Get Acceleration Data
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14);  // request a total of 14 registers

  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  float z_acceleration = /*1 -*/ abs(AcZ / 2048.0);
  float y_acceleration = /*1 -*/ (AcY / 2048.0);
  float x_acceleration = /*1 -*/ (AcX / 2048.0);

  if(z_acceleration == 0){z_acceleration = .001;}
  if(y_acceleration == 0){y_acceleration = .001;}
  if(x_acceleration == 0){x_acceleration = .001;}
  int angle = atan(z_acceleration / x_acceleration) * (180.0 / 3.14159);
  if (x_acceleration <= 0)
  {
      angle = 180 + angle;
  }

  // Vary The motor speed

  float motor_speed_0 = base_pwm + (200 * ((2 * (90 - angle)) / 90.0));
  if(motor_speed_0 < base_pwm - 50){motor_speed_0 = base_pwm - 50;}

  float motor_speed_1 = base_pwm + (200 * ((2 * (angle - 90)) / 90.0));
  if(motor_speed_1 < base_pwm - 50){motor_speed_1 = base_pwm - 50;}

  Serial.print(angle); Serial.print("\t");
  Serial.print(motor_speed_0); Serial.print("\t");
  Serial.println(motor_speed_1);


  esc_0.writeMicroseconds(motor_speed_0);
  esc_1.writeMicroseconds(motor_speed_1);

  delay(50);

}
