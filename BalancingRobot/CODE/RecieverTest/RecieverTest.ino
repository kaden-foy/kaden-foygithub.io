int CH1 = 13;
int CH2 = 12;
int CH3 = 8;
int CH4 = 7;

float st1_x;
float st1_y;
float st2_x;
float st2_y;

int SM_LU = 11;
int SM_LL = 10;
int SM_RU = 9;
int SM_RL = 6;

int SM_LU_val;
int SM_LL_val;
int SM_RU_val;
int SM_RL_val;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(CH1,INPUT);
  pinMode(CH2,INPUT);
  pinMode(CH3,INPUT);
  pinMode(CH4,INPUT);

  pinMode(SM_LU,OUTPUT);
  pinMode(SM_LL,OUTPUT);
  pinMode(SM_RU,OUTPUT);
  pinMode(SM_RL,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  st1_x = pulseIn(CH1,HIGH);
  st1_y = pulseIn(CH2,HIGH);
  st2_x = pulseIn(CH3,HIGH);
  st2_y = pulseIn(CH4,HIGH);

  SM_LU_val = map(st1_x, 987, 1962, 10,245);
  SM_LL_val = map(st1_y, 995, 1987, 10,245);
  SM_RU_val = map(st2_x, 996, 1990, 10,245);
  SM_RL_val = map(st2_y, 1013, 1999, 10,245);

  analogWrite(SM_LU,SM_LU_val);
  analogWrite(SM_LL,SM_LL_val);
  analogWrite(SM_RU,SM_RU_val);
  analogWrite(SM_RL,SM_RL_val);

  Serial.print(SM_LU_val); Serial.print(' ');
  Serial.print(SM_LL_val); Serial.print(' ');
  Serial.print(SM_RU_val); Serial.print(' ');
  Serial.println(SM_RL_val);

}
