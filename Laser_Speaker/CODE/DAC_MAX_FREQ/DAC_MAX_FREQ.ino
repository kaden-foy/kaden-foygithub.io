// import libaries needed
#include <Wire.h>
//#include <Adafruit_MCP4725.h> // get max frequancy of 13351Hz
#include "MCP4725.h"

// intilize dac
//Adafruit_MCP4725 DAC;
MCP4725 MCP(0x60);

void setup() {
  // start serial cummunication
  Serial.begin(9600);

  // create DAC connection
  //DAC.begin(0x60);
  
  Wire.begin();
  MCP.begin();
  Wire.setClock(800000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //DAC.setVoltage( 0, false); // 0V=0 and 5V=4095
  //DAC.setVoltage( 4095, false); // 0V=0 and 5V=4095

  MCP.setValue(0);
  delay(1);
  MCP.setValue(4095);
}
