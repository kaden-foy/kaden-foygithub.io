#include <Servo.h>

// Reading Radio Control
int CH1 = 13;
int CH2 = 12;
int CH3 = 8;
int CH4 = 7;

float st1_x;
float st1_y;
float st2_x;
float st2_y;

// Controlling servo motors
int SM_LU = 11;
int SM_LL = 10;
int SM_RU = 9;
int SM_RL = 6;

int SM_LU_val;
int SM_LL_val;
int SM_RU_val;
int SM_RL_val;

Servo LUservo;  // create servo object to control a servo
Servo LLservo;  // create servo object to control a servo
Servo RUservo;  // create servo object to control a servo
Servo RLservo;  // create servo object to control a servo

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(CH1,INPUT);
  pinMode(CH2,INPUT);
  pinMode(CH3,INPUT);
  pinMode(CH4,INPUT);

  LUservo.attach(SM_LU);
  LLservo.attach(SM_LL);
  RUservo.attach(SM_RU);
  RLservo.attach(SM_RL);
}

void loop() {
  // put your main code here, to run repeatedly:
  st1_x = pulseIn(CH1,HIGH);
  st1_y = pulseIn(CH2,HIGH);
  st2_x = pulseIn(CH3,HIGH);
  st2_y = pulseIn(CH4,HIGH);

  SM_LU_val = map(st1_x, 987, 1962, 0,180);
  SM_LL_val = map(st1_y, 995, 1987, 0,180);
  SM_RU_val = map(st2_x, 996, 1990, 0,180);
  SM_RL_val = map(st2_y, 1013, 1999, 0,180);

  LUservo.write(SM_LU_val);
  LLservo.write(SM_LL_val);
  RUservo.write(SM_RU_val);
  RLservo.write(SM_RL_val);
  delay(15); 

  Serial.print(SM_LU_val); Serial.print(' ');
  Serial.print(SM_LL_val); Serial.print(' ');
  Serial.print(SM_RU_val); Serial.print(' ');
  Serial.println(SM_RL_val);

}
