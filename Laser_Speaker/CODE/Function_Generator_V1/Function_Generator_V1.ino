/* Function Generator. This code generates a wave function
 * and then runs a optimized loop that plays the wave form.
 * This loop can be broken when a button is pressed
 * 
 * By Kaden foy
 * Date: Sep 19th 2021
 */

// libaries used
#include "MCP4725.h"
#include "Wire.h"

MCP4725 MCP(0x60);

// Global parameters
uint16_t   F[4] = {100.0, 30.0, 0.0, 0.0};
uint32_t   period = 0;

uint16_t   freq[4] = {50.0, 0.0, 0.0, 0.0};

// waveform 
uint16_t WAVE[360];

void setup() {
  // start serial comms
  Serial.begin(115200);

  // start DAC Comms
  Wire.begin();
  MCP.begin();
  Wire.setClock(800000);
  MCP.setValue(0);
}

void loop() {
  // this is an example of a full looped code
  // the code stays in waveloop until it needs
  // to update waveform and starts running in 
  // the main loop

  GenerateWave();

  WAVEFORM();

}

float SORT(uint16_t f[4]){
  // sorts frequancies from max to min
  

  return f
}

void GenerateWave() {
  // creates wave that is played. Takes arrays of 4 elements
  // that are ordered from hieght to lowest.

  freq[] = SORT(F);

  if (freq[3] != 0){ // 4 frequancies
    // fill table
    for (int i = 0; i < 361; i++)
    {
      WAVE[i] = 2047 + round(2047*sin(i*PI/180)*sin((freq[0]/freq[3])*i*PI/180)*sin((freq[1]/freq[3])*i*PI/180)*sin((freq[2]/freq[3])*i*PI/180));
    }
  }
  else if (freq[2] != 0){ // 3 frequancies
    // fill table
    for (int i = 0; i < 361; i++)
    {
      WAVE[i] = 2047 + round(2047*sin(i*PI/180)*sin((freq[0]/freq[2])*i*PI/180)*sin((freq[1]/freq[2])*i*PI/180));
    }
  }
  else if (freq[1] != 0){ // 2 frequancies
    // fill table
    for (int i = 0; i < 361; i++)
    {
      WAVE[i] = 2047 + round(2047*sin(i*PI/180)*sin((freq[0]/freq[1])*i*PI/180));
    }
  }
  else { // 1 frequancies
    // fill table
    for (int i = 0; i < 361; i++)
    {
      WAVE[i] = 2047 + round(2047*sin(i*PI/180));
    }
  }
}

void WAVEFORM() {
  // play waveform and ckeck button push
  yield();
  uint32_t now = micros();

  uint32_t t = now % period;

  int idx = (360 * t) / period;
  MCP.setValue(WAVE[idx]);  // lookuptable
}
