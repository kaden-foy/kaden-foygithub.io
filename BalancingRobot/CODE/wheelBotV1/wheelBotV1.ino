// This Code Controls the robot. It takes data
// from the fly sky reciever, accelerameter, 
// magnotometer, gryoscope (not working) and outputs 
// postions and velocitys for severo motors and 
// brushless motors.

// Kaden Foy
// June 13th 2021

// import needed modules ---------------------
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

// define pins -------------------------------
# define RDLED A0 // red LED (note on Analog pin)
# define GNLED A1 // Green LED (note on Analog pin)
# define YWLED A2 // red LED (note on Analog pin)

# define Xpin 13 // Reciever Pin for x postions
# define Ypin 12 // Reciever Pin for y postions
# define VFpin 8 // Reciever Pin for forward velocity 
# define VTpin 7 // Reciever Pin for Turn velocity

# define L_hip 10 // Left leg hip servo
# define L_knee 9 // Left leg knee servo
# define R_hip 5 // right leg hip servo
# define R_knee 3 // right leg knee servo

# define R_wheel 6 // Left leg hip servo
# define L_wheel 11 // Left leg hip servo

// Physical Dimensions -----------------------
#define thigh_L 150.0 // Length in mm
#define calf_L 142.8 // length in mm

// Define variables and objects --------------
// servo and motors
Servo L_hip_servo;  // create servo object to control a servo
Servo L_knee_servo;  // create servo object to control a servo
Servo R_hip_servo;  // create servo object to control a servo
Servo R_knee_servo;  // create servo object to control a servo

Servo R_wheel_motor;  // create servo object to control a brushless motor
Servo L_wheel_motor;  // create servo object to control a brushless motor

// Assign a unique ID to magnetometer at the same time
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

// Assign a unique ID to accelerometer at the same time
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

// 9 DOF postion sensor
float mag_x = 0; // magnetometer values
float mag_y = 0;
float mag_z = 0;

float acc_x = 0; // acceleration values
float acc_y = 0;
float acc_z = 0;

float gry_x = 0; // gryoscopic values
float gry_y = 0;
float gry_z = 0;

// reciever inputs
float x_pos = 0.0; // Inital x position
float y_pos = 290.0; // Inital x position

float VF = 0.0; // Iitall forward velocity
float VT = 0.0; // Iitall Turn velocity (negative is left, positive is right)

// Initalization Code -----------------------
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // start serial USB cummincations
  
  pinMode(RDLED,OUTPUT); // initalize LED pins
  pinMode(GNLED,OUTPUT);
  pinMode(YWLED,OUTPUT);

  pinMode(Xpin,INPUT); // Initalize Reciever pins
  pinMode(Ypin,INPUT);
  pinMode(VFpin,INPUT);
  pinMode(VTpin,INPUT);

  L_hip_servo.attach(L_hip); // Initalize Joint servos
  L_knee_servo.attach(L_knee);
  R_hip_servo.attach(R_hip);
  R_knee_servo.attach(R_knee);

  // set to zero postion (straight)
  //code

  R_wheel_motor.attach(R_wheel); // Initalize wheel motors
  L_wheel_motor.attach(L_wheel);

  // Set throttle to Zero
  //code

  mag.enableAutoRange(true); // auto set magnetometer 
  mag.begin(); // start magnetometer 

  accel.begin(); // start accelerometer communication
  
}

// Main Loop Code ------------------------------
void loop() {
  // put your main code here, to run repeatedly:

  Reciever(); // Update Control inputs
  /*
  Serial.print(x_pos);
  Serial.print(" ");
  Serial.print(y_pos);
  Serial.print(" ");
  Serial.print(VF);
  Serial.print(" ");
  Serial.println(VT);
  */
  
  Magnetometer(); // Update magnetometer outputs
 /*
  Serial.print(mag_x);
  Serial.print(" ");
  Serial.print(mag_y);
  Serial.print(" ");
  Serial.println(mag_z);
  */

  Accelerometer();
  /*
  Serial.print(acc_x);
  Serial.print(" ");
  Serial.print(acc_y);
  Serial.print(" ");
  Serial.println(acc_z);
  */

  LegPostion(3);

  MotorControl();

  Serial.print("LED");
  LEDOn(1,1,1);
  delay(100);
  LEDOn(0,0,0);
  delay(75);
}

// Control Functions --------------------------------
void LegPostion(int Leg) {
  // give X, Y, and leg postions and that required leg
  // will move to that postion. Uses Reverse kinematics
  // to find correct angles and then moves the motor 
  float r;
  float A1;
  float A2;
  
  r = sqrt(x_pos*x_pos+y_pos*y_pos); // calculate radius

  if (r >= thigh_L + calf_L){
    // fully extended
    A1 = atan(x_pos/y_pos);
    A2 = 0.0;
  }
  else if (r <= sqrt(thigh_L*thigh_L+calf_L*calf_L)){
    // under extended
    A1 = atan(x_pos/y_pos) - atan(calf_L/thigh_L);
    A2 = 3.1415/2.0;
  }
  else {
    // standard motion
    A1 = atan(x_pos/y_pos) - acos((r*r+thigh_L*thigh_L-calf_L*calf_L)/(2.0*r*thigh_L));
    A2 = 3.1415 - acos((thigh_L*thigh_L+calf_L*calf_L-r*r)/(2.0*thigh_L*calf_L));
  }

  // hyper extension backwards
  if (A1 <= -3.1415/2.0){
    A1 = -3.1415/2.0;
    if (thigh_L+x_pos>0.0){
      A2 = 3.1415/2.0;
    }
    else {
      A2 = atan(y_pos/(-x_pos-thigh_L));
    }
  }
  A1 = 180*A1/3.1415;
  A2 = 180*A2/3.1415;
  //A1 = -30;
  //A2 = -30;
/*
  Serial.print("position");
  Serial.print(x_pos);
  Serial.print(" ");
  Serial.print(y_pos);
  Serial.print(" ");
  Serial.print("angle");
  Serial.print(" ");
  Serial.print(A1);
  Serial.print(" ");
  Serial.println(A2);
 */
  if (Leg == -1) {
    L_hip_servo.write(-A1-5.0+90.0);
    L_knee_servo.write(-A2+90);
  }
  else if (Leg == 1) {
    R_hip_servo.write(180.0+A1-90.0);
    R_knee_servo.write(180.0+A2-90); 
  }
  else {
    L_hip_servo.write(-A1-5.0+90.0);
    L_knee_servo.write(-A2+90);
    R_hip_servo.write(180.0+A1-90.0);
    R_knee_servo.write(180.0+A2-90);
  }
}

void MotorControl() {
  // motor speed control
  VF = 120;
  VT = 0;
  Serial.print(VF);
  Serial.print(" ");
  Serial.println(VT);
  L_wheel_motor.write(VF-VT/2.0);
  R_wheel_motor.write(VF+VT/2.0);
  
}

// Sensor and Input Functions ---------------------------
void Accelerometer(){
  // update accelerometer values
  sensors_event_t event;
  accel.getEvent(&event);

  // scale and update acceleration sensor
  acc_x = event.acceleration.x;
  acc_y = event.acceleration.y;
  acc_z = event.acceleration.z;
}

// Magnetometer tuning param
#define xmag_min_param -200.0
#define xmag_max_param 300.0
#define ymag_min_param -200
#define ymag_max_param 300
#define zmag_min_param -200
#define zmag_max_param 300

void Magnetometer() {
  // update magnetometer values
  sensors_event_t event;
  mag.getEvent(&event);
  // scale calibrated field to useable values
  mag_x = map(mag.raw.x,xmag_min_param,xmag_max_param,-1000.0,1000.0); //event.magnetic.x;
  mag_y = map(mag.raw.y,ymag_min_param,ymag_max_param,-10.0,10.0);
  mag_z = map(mag.raw.z,zmag_min_param,zmag_max_param,-10.0,10.0);
}

// Reciever tuning param
#define x_min_param 997
#define x_max_param 1975
#define y_min_param 995
#define y_max_param 1989
#define vf_min_param 999
#define vf_max_param 1971
#define vt_min_param 1008
#define vt_max_param 1989

void Reciever() {
  // read reciever data and Update postions and
  // veocity values
  //Serial.println(pulseIn(Xpin,HIGH));
  x_pos = map(pulseIn(Xpin,HIGH),x_min_param,x_max_param,-thigh_L-calf_L,thigh_L+calf_L);
  y_pos = map(pulseIn(Ypin,HIGH),y_min_param,y_max_param,0.0,thigh_L+calf_L);
  VF = map(pulseIn(VFpin,HIGH),vf_min_param,vf_max_param,-20.0,20.0);
  VT = map(pulseIn(VTpin,HIGH),vt_min_param,vt_max_param,-10.0,10.0);

  if (y_pos <= 0.0){
    y_pos = 0.0;
  }

}

void LEDOn(int r, int y, int g){
  // turn on red yellow or green light. takes 
  // binary input
  digitalWrite(RDLED,r);
  digitalWrite(YWLED,y);
  digitalWrite(GNLED,g);
}
