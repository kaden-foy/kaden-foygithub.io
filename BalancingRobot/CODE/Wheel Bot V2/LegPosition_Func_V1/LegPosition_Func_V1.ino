/* Description: This Code takes a radius and angle about 
 * moves the legs to the correct location. The radius is
 * the length from the centre o the wheel to the hip joint.
 * The angle is between the radius and the body (not thigh).
 * 
 * By: Kaden Foy
 * Date: July 29th 2021
 */

// import modules needed
#include <Servo.h>

// Servo Pins used
# define L_hip 9 // Left leg hip servo
# define L_knee 10 // Left leg knee servo
# define R_hip 3 // right leg hip servo
# define R_knee 5 // right leg knee servo

// servo and motors
Servo L_hip_servo;  // create servo object to control a servo
Servo L_knee_servo;  // create servo object to control a servo
Servo R_hip_servo;  // create servo object to control a servo
Servo R_knee_servo;  // create servo object to control a servo

void setup() {
  // start serial BUS cummunication
  Serial.begin(9600);

  // Initalize Joint servos
  L_hip_servo.attach(L_hip); 
  L_knee_servo.attach(L_knee);
  R_hip_servo.attach(R_hip);
  R_knee_servo.attach(R_knee);  

}

void loop() {
  // example that moves the legs based on radius, lean, and tilt
  float R = 150.0;
  float L = -30.0/57.296;
  float T = 0.0/57.296;

  LegPosistion(R, L, T);
  delay(50);

}

void LegPosistion(float r, float l, float t){
  // function takes radius (r), lean (l), and tilt (t) and moves
  // the legs accordingly in mm and rads. Radius is the length between 
  // wheel and hip axis. Lean is the angle that the robot will 
  // lean left and right (cornering). Tilt is the angle of the 
  // main body with respect to the radius, 0 rads means legs
  // directly below.

  // calculate leg length difference for lean
  float d = 210.0*tan(l);

  // cases where tilting too much (+/- 60deg)
  if (abs(t)>1.0472){// out of bounds
    t = (t/abs(t))*1.0472;
  }

  // cases of over and under extending leg
  // 57.3 [deg/rad]
  if (r+abs(d)/2.0>290.0){// over extend (290mm or more)
    float dd = r+abs(d)/2.0 - 290.0;
    
    L_hip_servo.write(180.0-HipAngle(r-d/2.0-dd,t)*57.296 - 6.0); 
    L_knee_servo.write(180.0-KneeAngle(r-d/2.0-dd)*57.296);
    
    R_hip_servo.write(HipAngle(r+d/2.0-dd,t)*57.296 - 6.0);
    R_knee_servo.write(KneeAngle(r+d/2.0-dd)*57.296); 
    
  }
  else if (r-abs(d)/2.0<150.0){// under extend (150mm or less)
    float dd = 150.0 - (r-abs(d)/2.0);
    
    L_hip_servo.write(180.0-HipAngle(r-d/2.0+dd,t)*57.296 - 6.0); 
    L_knee_servo.write(180.0-KneeAngle(r-d/2.0+dd)*57.296);
    
    R_hip_servo.write(HipAngle(r+d/2.0+dd,t)*57.296 - 6.0);
    R_knee_servo.write(KneeAngle(r+d/2.0+dd)*57.296); 
    
  }
  else{
    
    L_hip_servo.write(180-HipAngle(r-d/2.0,t)*57.296 - 6.0); // correct miss alignment by 10 deg
    L_knee_servo.write(180-KneeAngle(r-d/2.0)*57.296);
    
    R_hip_servo.write(HipAngle(r+d/2.0,t)*57.296 - 6.0);
    R_knee_servo.write(KneeAngle(r+d/2.0)*57.296); 
  }
}

float HipAngle(float r, float t){
  // calculates angle used for hip serco motors
  float a1 = acos((2108.16+r*r)/(300.0*r)) + t;
  a1 = a1 + 1.5708; // correct to reference angle (90deg)
  return a1;
}

float KneeAngle(float r){
  // Calculates angles used for knee servo motors
  float a2 = acos(1.00121-r*r/42840.0);
  a2 = a2 - 1.5708; // correct to reference angle (90deg)
  return a2;
}
