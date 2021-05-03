#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

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
double Heading(){
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
  

double getHeading(){
  double heading = 0;
  while(heading == 0){
    heading = Heading();
  }
  return heading;
}

void turn_to_angle(int angle){
  heading = getHeading();

  int turn_right_amount = 0;
  int turn_left_amount = 0;

  if(heading < angle){
    turn_right_amount = angle - heading;
    turn_left_amount = 360 - (angle-heading);
  }
  if(heading > angle){
    turn_right_amount = 360-(heading-angle);
    turn_left_amount = heading-angle;
    }

  if(turn_right_amount < turn_left_amount){
    pwm.setPWM(4,0,power(0.1));
    pwm.setPWM(5,0,power(0.1));
  }else{
    pwm.setPWM(4,0,power(-0.1));
    pwm.setPWM(5,0,power(-0.1));
  }

  while((heading > (angle+2)) or (heading < (angle-2))){
    delay(1);
    heading = getHeading();
    Serial.println(heading);
    }
  pwm.setPWM(4,0,0);
  pwm.setPWM(5,0,0);


}

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

  turn_to_angle(0);
  Serial.println("0 Degrees");
  delay(1000);
  turn_to_angle(90);
  Serial.println("90 Degrees");
  turn_to_angle(180);
  delay(1000);
  turn_to_angle(270);
  delay(1000);
  turn_to_angle(0);

  
}

void loop(void) {
}
