
int L_Pin = 12;
int Pot_pin = A2;

float volt;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(L_Pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  volt = analogRead(Pot_pin)/2.0;
  Serial.println(volt);
  digitalWrite(L_Pin, HIGH);
  delay(volt);
  digitalWrite(L_Pin, LOW);
  delay(volt);
}
