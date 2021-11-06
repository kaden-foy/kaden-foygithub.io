/* Description: this code controls the 3 LEDs on the robot.
 * It includes pins used, and an example of the function 
 * created that controls all 3 colors
 * 
 * By: Kaden Foy
 * Date: July 28th 2021
 */

 // LED pins used
# define RDLED A0 // red LED (note on Analog pin)
# define GNLED A1 // Green LED (note on Analog pin)
# define YWLED A2 // red LED (note on Analog pin)

void setup() {
  // initalize LED pins
  pinMode(RDLED,OUTPUT); 
  pinMode(GNLED,OUTPUT);
  pinMode(YWLED,OUTPUT);
}

void loop() {
  // example of turning LED lights R, Y, G on then off with 0.25S spacing
  // LED_On(RED,YELLOW,GREEN);
  LED_On(1,0,0);
  delay(250);
  LED_On(0,1,0);
  delay(250);  
  LED_On(0,0,1);
  delay(250);
}

void LED_On(int r, int y, int g){
  // function that turns on the LED where pin value is set high
  digitalWrite(RDLED,r);
  digitalWrite(YWLED,y);
  digitalWrite(GNLED,g);
}
