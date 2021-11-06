// Calibrations for ESC
#include <Servo.h>

# define R_wheel 6 // Left leg hip servo
# define L_wheel 11 // Left leg hip servo

Servo R_wheel_motor;  // create servo object to control a brushless motor
Servo L_wheel_motor;  // create servo object to control a brushless motor

#define Ypin 12 // Reciever Pin for y postions
#define y_min_param 995
#define y_max_param 1989

float throttle;
// global variables (tracks initial change in direction)
byte R_dir = 1; 
byte L_dir = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // start serial USB cummincations

  throttle = 0.0;
  
  R_wheel_motor.attach(R_wheel); // Initalize wheel motors
  L_wheel_motor.attach(L_wheel);

}

void loop() {
  // put your main code here, to run repeatedly:

  // set ESC throttle to max (180deg)
  RightWheelSpeed(throttle);
  LeftWheelSpeed(throttle);

  throttle = map(pulseIn(Ypin,HIGH),y_min_param,y_max_param,0,180);

  Serial.print("throttle = ");
  Serial.println(throttle);
  

}

void RightWheelSpeed(float Speed){
  // check if wheel is changing from forward to reverse
  if (Speed < 90){
    // set to reverse
    if (R_dir == 1){
      // Secuqence used to reverse direction
      delay(10);
      R_wheel_motor.write(75); 
      delay(10);
      R_wheel_motor.write(95);
      delay(10);
      R_wheel_motor.write(75);
    }
    R_dir = 0;
  }
  else{
    // set to forward
    R_dir = 1;
  }
  
  // creates a double pull pack on throttle effect to bypass the brake
  R_wheel_motor.write(Speed); 
}

void LeftWheelSpeed(float Speed){
  // check if wheel is changing from forward to reverse
  if (Speed < 90){
    // set to reverse
    if (L_dir == 1){
      // Secuqence used to reverse direction
      delay(10); 
      L_wheel_motor.write(0); 
      delay(10);
      L_wheel_motor.write(91);
    }
    L_dir = 0;
  }
  else{
    // set to forward
    L_dir = 1;
  }
  
  // creates a double pull pack on throttle effect to bypass the brake
  L_wheel_motor.write(Speed); 
}
