/* This code generates a wave form and outputs it 
 *  to a 12bit DAC. This DAC controlls a speaker.
 *  
 *  By Kaden, Aigne
 *  Date 13/08/2021
 */

// import libaries needed
#include <Wire.h>
#include <Adafruit_MCP4725.h>

// Pin Layout
//#define LED1 1 // example of pincode

// Global Variables
float S1; // singal one
float F1; // frequancy of singal one
float Pi1; // phase of signal one
float Amp1; // amplitube of singal one
float S2; // singal two
float F2; // frequancy of singal two
float Pi2; // phase of signal Two
float Amp2; // amplitube of singal two

float Time;

// intilize dac
Adafruit_MCP4725 DAC;


void setup() {
  // start serial cummunication
  Serial.begin(9600);

  // create DAC connection
  DAC.begin(0x60);

  //for testing
  F1 = 15.0; // frequancy of singal one
  Pi1 = 0.; // phase of signal one
  Amp1 = 1.0; // amplitube of singal one
  F2 = 0.0; // frequancy of singal two
  Pi2 = 0.0; // phase of signal Two
  Amp2 = 0.0; // amplitube of singal two
  
}

void loop() {
  // Code takes POT and switch inputs and outputs a sine
  // made of its frequancy, amplitude proporations and phase

  SetDAC();

}

void SetDAC(){
  // takes frequancy and mixes them
  Time = millis()/1000.0;
  
  S1 = Amp1*4096.0*(0.5+0.5*sin(6.2832*F1*Time+Pi1))/(Amp1+Amp2);
  S2 = Amp2*4096.0*(0.5+0.5*sin(6.2832*F2*Time+Pi2))/(Amp1+Amp2);

  DAC.setVoltage( int16_t(S1+S2), false); // 0V=0 and 5V=4095

  //Serial.println(int16_t(S1+S2));
}
