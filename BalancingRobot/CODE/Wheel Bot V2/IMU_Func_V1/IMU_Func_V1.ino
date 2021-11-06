/* Description: This code reads the IMU and computes the
 * orientation of it in 3D space
 * 
 * By: Kaden Foy
 * Date: July 28th 2021
 */

/*
 * These are the address found on the BUS
 * I2C device found at address 0x19
 * I2C device found at address 0x1E
 * I2C device found at address 0x69
 */

// IMU modules used
#include <SPI.h>
#include <Wire.h>
    
#include <Adafruit_Sensor.h>
//#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_9DOF.h>

// Assigning a unique ID to these sensors
//Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(1);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(2);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(3);
//Adafruit_9DOF                 dof   = Adafruit_9DOF();

// 
struct IMU_Val {
float mag_x = 0; // magnetometer values
float mag_y = 0;
float mag_z = 0;

float acc_x = 0; // acceleration values
float acc_y = 0;
float acc_z = 0;

float gry_x = 0; // gryoscopic values
float gry_y = 0;
float gry_z = 0;
};

IMU_Val Position;

void setup() {
  // start serial BUS cummunication
  Serial.begin(9600);
  delay(30);
  // Initialise the sensor 
  if(!accel.begin()){
    // There was a problem detecting the LSM303 ... check your connections
    Serial.println("Ooops accel, no LSM303 detected ... Check your wiring!");
    //while(1);
  }

  // Enable auto-gain
  mag.enableAutoRange(true);

  // Initialise the sensor
  if(!mag.begin()){
    // There was a problem detecting the LSM303 ... check your connections
    Serial.println("Ooops mag, no LSM303 detected ... Check your wiring!");
    //while(1);
  }

  /*
  gyro.enableAutoRange(true);
  gyro.begin();
  */
}

void loop() {
  /* Get a new sensor event */ 
  IMU();

  
  // Display the results (acceleration is measured in m/s^2)
  Serial.print("X: "); Serial.print(Position.acc_x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Position.acc_y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Position.acc_z); Serial.print("  ");Serial.println("m/s^2 ");
  /*
  // Display the results 
  Serial.print("X: "); Serial.print(event.gyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.gyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.gyro.z); Serial.println("  ");
  
  // Display the results
  Serial.print("X: "); Serial.print(Position.mag_x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Position.mag_y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Position.mag_z); Serial.println("  ");
  */
}

void IMU(){
  sensors_event_t event;
  mag.getEvent(&event);
  // magnetometer values
  Position.mag_x = event.magnetic.x; 
  Position.mag_y = event.magnetic.y;
  Position.mag_z = event.magnetic.z;

  accel.getEvent(&event);
  Position.acc_x = event.acceleration.x; 
  Position.acc_y = event.acceleration.y;
  Position.acc_z = event.acceleration.z;
}
