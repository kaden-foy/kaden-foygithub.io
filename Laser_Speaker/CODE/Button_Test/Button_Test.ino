int B1_pin = 9;
int B2_pin = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(B1_pin,INPUT);
  pinMode(B2_pin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(B1_pin)==1){
    Serial.println("BUTTON 1 PUSHED");
  }
  else if (digitalRead(B2_pin)==1){
    Serial.println("BUTTON 2 PUSHED");
  }
  delay(5);
}
