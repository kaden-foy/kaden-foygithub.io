//
//    FILE: MCP4725_wave_generator.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: demo function generators
//    DATE: 2021-01-07
//     URL: https://github.com/RobTillaart/FunctionGenerator
//
//  depending on the platform, the range of "smooth" sinus is limited.
//  other signals are less difficult so have a slightly larger range.
//
//  PLATFORM     SINUS    SQUARE  SAWTOOTH  TRIANGLE
//  UNO          -100 Hz  
//  ESP32        -200 Hz  -1000   -250      -100
//

#include "MCP4725.h"
#include "Wire.h"

uint16_t   freq = 100;
uint32_t   period = 0;

MCP4725 MCP(0x60);
uint32_t lastTime = 0;

// LOOKUP TABLE SINE
uint16_t sine[360];

// Laser Pin
int LASER = 12;

void setup()
{
  Serial.begin(115200);
  
  // Turn Laser off
  digitalWrite(LASER,LOW);

  // fill table
  for (int i = 0; i < 361; i++)
  {
    sine[i] = 2047 + round(2047 * sin(i * PI / 180) * sin(2.5 * i * PI / 180)* sin(5 * i * PI / 180));
  }

  Wire.begin();
  MCP.begin();
  Wire.setClock(800000);

  MCP.setValue(0);

  period = 1e6 / freq;
  
}

void loop()
{
  yield();
  uint32_t now = micros();

  uint32_t t = now % period;

  int idx = (360 * t) / period;
  MCP.setValue(sine[idx]);  // lookuptable
}

// END OF FILE
