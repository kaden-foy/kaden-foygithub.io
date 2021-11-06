#include <Wire.h>
#include "MCP4725.h"

MCP4725 MCP(0x60);

// Pot Pin
int POT = A2;

uint16_t   freq = 0000;
uint32_t   period = 0;

int LASER = 12;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello!");
  Wire.begin();
  MCP.begin();
  Wire.setClock(800000);
  Serial.println("Hello!!");
  pinMode(LASER, OUTPUT); 
  digitalWrite(LASER,LOW); // pulling pin high turns laser off (LOW=on)
  Serial.println("Hello!!!");
}

void loop() {
  yield();
  uint32_t now = micros();
    
  freq = analogRead(POT);
  period = 1e6 / freq;

  uint32_t t = now % period;
  
  float f = ((PI * 2) * t)/period;
  MCP.setValue(2047 + 2047 * sin(f));

}
