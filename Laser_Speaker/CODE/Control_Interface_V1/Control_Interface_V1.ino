// Interface

// Arduino Pins
// Channel LED Pins
int CH_1 = 2;
int CH_2 = 3;
int CH_3 = 4;
int CH_4 = 5;

// Tuning LED Pins
int TUNE_p = 6;
int TUNE_a = 7;
int TUNE_f = 8;

// Button Pins
int CH_B = 10;
int TUNE_B = 9;

// Laser Pin
int LASER = 12;

// Pot Pin
int POT = A2;

// Variables
float F[4] = {0.0, 0.0, 0.0, 0.0};
float A[4] = {1.0, 0.0, 0.0, 0.0};
float A_int[4] = {1023, 0, 0, 0};// amplitude not normilized
float P[4] = {0.0, 0.0, 0.0, 0.0};

void setup() {
  // intilize serial cummunication
  Serial.begin(9600);

  // define LED Pins
  pinMode(CH_1, OUTPUT);
  pinMode(CH_2, OUTPUT);
  pinMode(CH_3, OUTPUT);
  pinMode(CH_4, OUTPUT);
  pinMode(TUNE_f, OUTPUT);
  pinMode(TUNE_a, OUTPUT);
  pinMode(TUNE_p, OUTPUT);  
  pinMode(LASER, OUTPUT); 

  // define Button Pins
  pinMode(CH_B,INPUT);
  pinMode(TUNE_B,INPUT);

  // Turn Laser off
  digitalWrite(LASER,HIGH); // pulling pin high turns laser off (LOW=on)

  // flash lights
  flashLights();
}

void loop() {
  // The main loop of code is to run give outputs to DAC
  // Therefor it only checks and updates the frequancies 
  // when required

  main_code_example();

  Interface();

}

void Interface() {
  // check if either button has been pressed
  if (digitalRead(CH_B)+digitalRead(TUNE_B)>0){
    hold(); // wait till button is released
    // Interface control 
    int CH_N = 0; // set channel count the first channel
    int TUNE_N = 0; // Set Tuner to frequancy
    
    while (CH_N<4){
      // while channel is less loop through and allow user to 
      // change frequancy, amplitude, and phase of each channel.
      Serial.print("Channel ");
      Serial.print(CH_N);
      Serial.print(" Tunner ");
      Serial.print(TUNE_N);
      Serial.print(" Freq ");
      Serial.print(F[0]);
      Serial.print(F[1]);
      Serial.print(F[2]);
      Serial.print(F[3]);
      Serial.print(" Amp  ");
      Serial.print(A[0]);
      Serial.print(A[1]);
      Serial.print(A[2]);
      Serial.print(A[3]);
      Serial.print(" phi  ");
      Serial.print(P[0]);
      Serial.print(P[1]);
      Serial.print(P[2]);
      Serial.println(P[3]);

      if (TUNE_N==0){// update frequancy value
        F[CH_N] = analogRead(POT); // Frequancy between 0 and 1023HZ (This probabaly isnt true)
        digitalWrite(CH_1,LOW);
        digitalWrite(CH_2,LOW);
        digitalWrite(CH_3,LOW);
        digitalWrite(CH_4,LOW);
        digitalWrite(5-CH_N,HIGH);
        digitalWrite(TUNE_f,HIGH);
        digitalWrite(TUNE_a,LOW);
        digitalWrite(TUNE_p,LOW);
      }
      else if (TUNE_N==1){ // update amplitude value
        A_int[CH_N] = analogRead(POT);
        for (int i=0; i<4; i++){
          A[i] = 4096*A_int[i]/(A_int[0]+A_int[1]+A_int[2]+A_int[3]); // normalize amplitude for 12 bit dac
        }
        digitalWrite(CH_1,LOW);
        digitalWrite(CH_2,LOW);
        digitalWrite(CH_3,LOW);
        digitalWrite(CH_4,LOW);
        digitalWrite(5-CH_N,HIGH);
        digitalWrite(TUNE_f,LOW);
        digitalWrite(TUNE_a,HIGH);
        digitalWrite(TUNE_p,LOW);
      }
      else { // update phase value
        P[CH_N] = analogRead(POT)/162.8155; // Phase bewteen 0 and 2pi
        digitalWrite(CH_1,LOW);
        digitalWrite(CH_2,LOW);
        digitalWrite(CH_3,LOW);
        digitalWrite(CH_4,LOW);
        digitalWrite(5-CH_N,HIGH);
        digitalWrite(TUNE_f,LOW);
        digitalWrite(TUNE_a,LOW);
        digitalWrite(TUNE_p,HIGH);
      }
      // check button press
      if (digitalRead(CH_B)==1){
        CH_N ++;
        hold();
      }
      if (digitalRead(TUNE_B)==1){
        TUNE_N ++;
        if (TUNE_N >2){// roll over tuner model
          TUNE_N = 0;
        }
        hold();
      }
      
    }
    // Turn off LEDS
    digitalWrite(CH_1,LOW);
    digitalWrite(TUNE_f,LOW);
    digitalWrite(TUNE_a,LOW);
    digitalWrite(TUNE_p,LOW);
  }
}

void hold(){
  // This function holds the code until the button has been released
  while (digitalRead(CH_B)+digitalRead(TUNE_B)>0){
    delay(10);
  }
}

void flashLights(){
  for (int i = 0; i <=5; i++){
    digitalWrite(CH_1,HIGH);
    digitalWrite(CH_2,HIGH);
    digitalWrite(CH_3,HIGH);
    digitalWrite(CH_4,HIGH);
    digitalWrite(TUNE_f,HIGH);
    digitalWrite(TUNE_a,HIGH);
    digitalWrite(TUNE_p,HIGH);
    delay(200);
    digitalWrite(CH_1,LOW);
    digitalWrite(CH_2,LOW);
    digitalWrite(CH_3,LOW);
    digitalWrite(CH_4,LOW);
    digitalWrite(TUNE_f,LOW);
    digitalWrite(TUNE_a,LOW);
    digitalWrite(TUNE_p,LOW);
    delay(200);
  }
}

void main_code_example() {
  Serial.println("Running Main Code");
  delay(50);
}
