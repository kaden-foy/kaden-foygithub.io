/* This code controls the motor speed. It converts the 
 * speed controllers system of brake then double tap 
 * backwards to a simple speed output. It also has a
 * speed change built in so the motors cant be set full
 * throtte forward then instally put full throttle in 
 * reverse.
 * 
 * By: Kaden Foy
 * Date: August 2nd 2021
 */

// import modules needed
#include <Servo.h>
//#include <Reciever_Func.h>

// Servo Pins used
# define R_wheel 6 // Left leg hip servo
# define L_wheel 11 // Left leg hip servo

// global variables (tracks initial change in direction)
byte R_dir = 1; 
byte L_dir = 1;

// servo and motors
Servo R_wheel_motor;  // create servo object to control a brushless motor
Servo L_wheel_motor;  // create servo object to control a brushless motor

void setup() {
  // start serial BUS cummunication
  Serial.begin(9600);

  // Initalize wheel motors
  R_wheel_motor.attach(R_wheel); 
  L_wheel_motor.attach(L_wheel);

}

void loop() {
  // eample that spins the motor in a sin wave going forward and reverse
  float Speed = 90.0 + 25.0*sin(0.0005*millis()); // create speed from 0 to 180 (90 being 0 rpm)
  float Turn = 0.0;

  WheelControl(Speed,Turn);
  
}

void WheelControl(float Speed, float turn){
  // takes speed from 0 to 180 for throttle control.
  // Value of 90 means 0 speed.
  // Turn goes from -90 to 90

  if (Speed + abs(turn)>180.0){// turning wheel to fast while going forward
    float dd = (180 - (Speed + abs(turn)))/2.0;
    RightWheelSpeed(Speed + turn/2.0 + dd); 
    LeftWheelSpeed(Speed - turn/2.0 + dd);
  }
  else if (Speed - abs(turn)<0.0){// turning wheel to fast while going reverse
    float dd = (Speed - abs(turn))/2.0;
    RightWheelSpeed(Speed + turn/2.0 - dd); 
    LeftWheelSpeed(Speed - turn/2.0 - dd);
  }
  else{// turn is within bounds 0-180 for both wheels
    RightWheelSpeed(Speed + turn/2.0); 
    LeftWheelSpeed(Speed - turn/2.0);
  }
}

void RightWheelSpeed(float Speed){
  // check if wheel is changing from forward to reverse
  if (Speed < 90){
    // set to reverse
    if (R_dir == 1){
      Serial.println("");
      Serial.println(" Right wheel changing direction");
      //delay(1000);
      // Secuqence used to reverse direction
      delay(10);
      R_wheel_motor.write(75); 
      delay(10);
      R_wheel_motor.write(95);
      //delay(10); 
    }
    R_dir = 0;
  }
  else{
    // set to forward
    R_dir = 1;
  }
  
  // creates a double pull pack on throttle effect to bypass the brake
  R_wheel_motor.write(Speed); 

  Serial.print("Right Wheel ");
  Serial.print(Speed);
}

void LeftWheelSpeed(float Speed){
  // check if wheel is changing from forward to reverse
  if (Speed < 90){
    // set to reverse
    if (L_dir == 1){
      Serial.println("");
      Serial.println(" Left wheel changing direction");
      delay(1000);
      // Secuqence used to reverse direction
      L_wheel_motor.write(0); 
      // delay(10);
      L_wheel_motor.write(91);
      // delay(10); 
    }
    L_dir = 0;
  }
  else{
    // set to forward
    L_dir = 1;
  }
  
  // creates a double pull pack on throttle effect to bypass the brake
  L_wheel_motor.write(Speed); 

  Serial.print(" Left Wheel ");
  Serial.println(Speed);
}
