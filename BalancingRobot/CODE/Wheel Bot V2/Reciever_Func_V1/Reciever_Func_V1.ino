/* Description: This Code Reads the Raw data from the radio 
 * reciever and outputs scaled values. The output ranges 
 * from 0 to 100 for both xl,yl,xr, and yr (left and right).
 * Note that this range is a calibration range so it can
 * drift above and below the range.
 * 
 * By: Kaden Foy
 * Date: July 28th 2021
 */

// Reciever Pins used
# define XLpin 13 // Reciever Pin for x postions
# define YLpin 12 // Reciever Pin for y postions
# define XRpin 7 // Reciever Pin for forward velocity 
# define YRpin 8 // Reciever Pin for Turn velocity

struct control { // structor containing all outputs for reciever
  float XL;
  float YL;
  float XR;
  float YR;
};
control con = {0.0, 0.0, 0.0, 0.0};

void setup() {
  // Initalize Reciever pins
  pinMode(XLpin,INPUT); 
  pinMode(YLpin,INPUT);
  pinMode(XRpin,INPUT);
  pinMode(YRpin,INPUT);

  // start serial BUS cummunication
  Serial.begin(9600);
  
}

void loop() {
  // example of using Reciever function by output scaled values to serial port

  // update Reciever values
  Reciever();

  // print results
  Serial.print("xl = ");
  Serial.print(con.XL);
  Serial.print(" yl = ");
  Serial.print(con.YL);
  Serial.print(" xr = ");
  Serial.print(con.XR);
  Serial.print(" yr = ");
  Serial.println(con.YR);

}

void Reciever(){
  // When called it updated the reciever values and returns
  // the 4 joystick values xl, yl, xr, and yr which have been
  // scaled to 0 and 100

  // read signal
  int xl = pulseIn(XLpin,HIGH);
  int yl = pulseIn(YLpin,HIGH);
  int xr = pulseIn(XRpin,HIGH);
  int yr = pulseIn(YRpin,HIGH);
  
  // scale signal and update structor
  con.XL = Map_Val(xl,989,1960,0.0,100);
  con.YL = Map_Val(yl,987,1974,0.0,100);
  con.XR = Map_Val(xr,1013,1989,0.0,100);
  con.YR = Map_Val(yr,996,1984,0.0,100);
}

float Map_Val(float val, float p1, float p2, float v1, float v2){
  float slope = (v2-v1)/(p2-p1);
  float offset = v1 - slope*p1;
  float value = slope*val+offset;

  return value;
}
