#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

int left_motor = 4;
int right_motor = 5;

int drive_angle = 90;

float controlled_variable = 0.15;
float kp = .003;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//determine the pulse length in microseconds from a percent power(+ is forward,, - is backward)
int power(double power){
  if(power > 0){
    return(1450*power + 2495);
  }
  if(power < 0){
    return(1450*power + 2470);
  }
  if(power == 0){
    return 2500;
  }
}
float Pi = 3.14159;
float heading = 359;

Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);


void setup(void) {
  Serial.begin(115200);
  Serial.println("Magnetometer Test");
  Serial.println("");
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(400);  // This is the maximum PWM frequency  
  Wire.setClock(400000);


  /* Initialise the sensor */
  if (!mag.begin()) {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }

  turn_to_angle(drive_angle);                                                               
}

void loop(void) {
  
  pwm.setPWM(left_motor,0,power(0.15));
  pwm.setPWM(5,0,power(-controlled_variable));

  move(0);
  exit;
}


double getHeading(){
  double heading = 0;
  sensors_event_t event;
  mag.getEvent(&event);
  // Calculate the angle of the vector y,x
  heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / Pi;
  // Normalize to 0-360
  if (heading < 0) {
    heading = 360 + heading;
  }//offset of ~3 degrees
  return heading;
}

void turn_to_angle(int angle){
  heading = getHeading();

  int opposite_angle = angle + 180;
  if(angle > 360){ opposite_angle -= 360; }
  
  if(heading < opposite_angle){
      pwm.setPWM(left_motor,0,power(-0.1));
      pwm.setPWM(right_motor,0,power(-0.1));
  }
  else{
      pwm.setPWM(left_motor,0,power(0.1));
      pwm.setPWM(right_motor,0,power(0.1));
  }
    
  while(((heading < (angle - 5)) or (heading > (angle + 5  ))) or (heading == 0)){
    delay(1);
    heading = getHeading();
    Serial.println(heading);
    }
    
  pwm.setPWM(4,0,0);
  pwm.setPWM(5,0,0);


}

void move(int angle){

  int opposite_angle = angle + 180;
  if(opposite_angle >= 360){ opposite_angle -= 360; }   

  int lower_limit = angle - 5;
  int upper_limit = angle + 5;
  
  if(lower_limit < 0){ lower_limit = 360 + lower_limit; }
  if(upper_limit >= 360){ upper_limit -= 360; }
  
  heading = getHeading();
  Serial.print("Heading: "); Serial.println(heading);
  if(((heading > upper_limit) and (heading <  opposite_angle)) or ((heading < lower_limit) and (heading >  opposite_angle))){
    if(heading < opposite_angle){ controlled_variable += (heading*kp)/100; }
    if(heading > opposite_angle){ controlled_variable -= ((360-heading)*kp)/100; }
  }else{
    controlled_variable = 0.15;
  }
  if(controlled_variable > .3){ controlled_variable = 0.3; }
  if(controlled_variable < -.3){ controlled_variable = -0.3; }
  Serial.println(controlled_variable);
  delay(10);
}
