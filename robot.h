#include "Adafruit_VL53L0X.h"
#include <math.h>
#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Encoder.h>
Encoder myEnc(12, 13);

#ifndef robot
#define robot

int left_motor = 4;
int right_motor = 5;
float Pi = 3.14159;

int robot_x = 0;
int robot_y = 0;

int up_dub = 700;
int right_dub = 700;
int down_dub = 700;
int left_dub = 700;

Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//degree variables cause arrays and strings in C++ suck
int deg_0;int deg_20;int deg_40;int deg_60;int deg_80;int deg_100;int deg_120;int deg_140;int deg_160;int deg_180;int deg_200;int deg_220;int deg_240;int deg_260;int deg_280;int deg_300;int deg_320;int deg_340;

//convert from deg to radians on an equivalent spot on a unit circle
double conversion(int angle){
  double rads;
  if (angle < 90){
    rads = ((-angle + 90)*3.14159)/180;
  } else{
    rads = ((-angle + 450)*3.14159)/180;
  }
  return rads;
}

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

void drive_left_motor(double pwr){ pwm.setPWM(left_motor,0,power(pwr));}
void drive_right_motor(double pwr){ pwm.setPWM(right_motor,0,power(-pwr));}

void left_stop(){ pwm.setPWM(left_motor,0,0); }
void right_stop(){ pwm.setPWM(right_motor,0,0); }

float Heading() {
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);

  float Pi = 3.14159;

  // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / Pi;

  // Normalize to 0-360
  if (heading < 0) {
    heading = 360 + heading;
  }//offset of ~3 degrees
  return heading;
}

float getHeading(){
  float heading = 0;
  while(heading == 0){
    heading = Heading();
  }
  return heading;
}

void turn_to_angle(int angle){
  double heading = getHeading();

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

  while((heading > (angle+3)) or (heading < (angle-3))){
    heading = getHeading();
    }
  pwm.setPWM(4,0,0);
  pwm.setPWM(5,0,0);
}

int getDistance(){
  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  int distance = measure.RangeMilliMeter;
  int true_distance;
  
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    if (measure.RangeMilliMeter < 510){
      return distance;
    }
    if (measure.RangeMilliMeter >= 510 and measure.RangeMilliMeter < 710){
        true_distance = pow((distance-200),2);
        true_distance = true_distance*0.0005;
        true_distance = true_distance + 10;
        return distance;
    }
    if (measure.RangeMilliMeter >= 710) {
     return 0;
    }
      
  } else {
    return 0;
  }
}

int distance_average(){
  int avg = 0;
  int count = 0;
  for(int i=0;i<25;i++){
    int distance = getDistance();
    if(distance != 0){
      count++;
      avg += distance;
    }
  }
  if(count != 0){
    return(avg /= count);
  }
  else{
    return 0;
  }
}

double trig_x(int distance, int angle){ return abs(cos(conversion(angle))*distance); }
double trig_y(int distance, int angle){ return abs(sin(conversion(angle))*distance); }



void eval_up_dub(){
  if((trig_x(deg_320,320) < 25) and (trig_x(deg_320,320) != 0)){
    if((trig_y(deg_320,320) < up_dub) and (trig_y(deg_320,320) != 0)){ up_dub = trig_y(deg_320,320); }
  }
  if((trig_x(deg_340,340) < 25) and (trig_x(deg_340,340) != 0)){
    if((trig_y(deg_340,340) < up_dub) and (trig_y(deg_340,340) != 0)){ up_dub = trig_y(deg_340,340); }
  }
  if((trig_y(deg_0,0) < up_dub) and (trig_y(deg_0,0) != 0)){ up_dub = trig_y(deg_0,0); }

  if((trig_x(deg_20,20) < 25) and (trig_x(deg_20,20) != 0)){
    if((trig_y(deg_20,20) < up_dub) and (trig_y(deg_20,20) != 0)){ up_dub = trig_y(deg_20,20); }
  }
  if((trig_x(deg_40,40) < 25) and (trig_x(deg_40,40) != 0)){
    if((trig_y(deg_40,40) < up_dub) and (trig_y(deg_40,40) != 0)){ up_dub = trig_y(deg_40,40); }
  }
  if((trig_x(deg_60,60) < 25) and (trig_x(deg_60,60) != 0)){
    if((trig_y(deg_60,60) < up_dub) and (trig_y(deg_60,60) != 0)){ up_dub = trig_y(deg_60,60); }
  }
}

void eval_right_dub() {
    if ((trig_y(deg_60, 60) < 25) and (trig_y(deg_60, 60) != 0)) {
        if ((trig_x(deg_60, 60) < right_dub) and (trig_x(deg_60, 60) != 0)) { right_dub = trig_x(deg_60, 60); }
    }
    if ((trig_y(deg_80, 80) < 25) and (trig_y(deg_80, 80) != 0)) {
        if ((trig_x(deg_80, 80) < right_dub) and (trig_x(deg_80, 80) != 0)) { right_dub = trig_x(deg_80, 80); }
    }
    if ((trig_y(deg_100, 100) < 25) and (trig_y(deg_100, 100) != 0)) {
        if ((trig_x(deg_100, 100) < right_dub) and (trig_x(deg_100, 100) != 0)) { right_dub = trig_x(deg_100, 100); }
    }
    if ((trig_y(deg_120, 120) < 25) and (trig_y(deg_120, 120) != 0)) {
        if ((trig_x(deg_120, 120) < right_dub) and (trig_x(deg_120, 120) != 0)) { right_dub = trig_x(deg_120, 120); }
    }
    if ((trig_y(deg_140, 140) < 25) and (trig_y(deg_140, 140) != 0)) {
        if ((trig_x(deg_140, 140) < right_dub) and (trig_x(deg_140, 140) != 0)) { right_dub = trig_x(deg_140, 140); }
    }
 
}

void eval_down_dub() {
    if ((trig_x(deg_140, 140) < 25) and (trig_x(deg_140, 140) != 0)) {
        if ((trig_y(deg_140, 140) < down_dub) and (trig_y(deg_140, 140) != 0)) { down_dub = trig_y(deg_140, 140); }
    }
    if ((trig_x(deg_160, 160) < 25) and (trig_x(deg_160, 160) != 0)) {
        if ((trig_y(deg_160, 160) < down_dub) and (trig_y(deg_160, 160) != 0)) { down_dub = trig_y(deg_160, 160); }
    }
    
    if ((trig_y(deg_180, 180) < down_dub) and (trig_y(deg_180, 180) != 0)) { down_dub = trig_y(deg_180, 180); }

    if ((trig_x(deg_200, 200) < 25) and (trig_x(deg_200, 200) != 0)) {
        if ((trig_y(deg_200, 200) < down_dub) and (trig_y(deg_200, 200) != 0)) { down_dub = trig_y(deg_200, 200); }
    }
    if ((trig_x(deg_220, 220) < 25) and (trig_x(deg_220, 220) != 0)) {
        if ((trig_y(deg_220, 220) < down_dub) and (trig_y(deg_220, 220) != 0)) { down_dub = trig_y(deg_220, 220); }
    }
    if ((trig_x(deg_240, 240) < 25) and (trig_x(deg_240, 240) != 0)) {
        if ((trig_y(deg_240, 240) < down_dub) and (trig_y(deg_240, 240) != 0)) { down_dub = trig_y(deg_240, 240); }
    }
}

void eval_left_dub() {
    if ((trig_y(deg_240, 240) < 25) and (trig_y(deg_240, 240) != 0)) {
        if ((trig_x(deg_240, 240) < left_dub) and (trig_x(deg_240, 240) != 0)) { left_dub = trig_x(deg_240, 240); }
    }
    if ((trig_y(deg_260, 260) < 25) and (trig_y(deg_260, 260) != 0)) {
        if ((trig_x(deg_260, 260) < left_dub) and (trig_x(deg_260, 260) != 0)) { left_dub = trig_x(deg_260, 260); }
    }

    if ((trig_y(deg_280, 280) < 25) and (trig_y(deg_280, 280) != 0)) {
        if ((trig_x(deg_280, 280) < left_dub) and (trig_x(deg_280, 280) != 0)) { left_dub = trig_x(deg_280, 280); }
    }
    if ((trig_y(deg_300, 300) < 25) and (trig_y(deg_300, 300) != 0)) {
        if ((trig_x(deg_300, 300) < left_dub) and (trig_x(deg_300, 300) != 0)) { left_dub = trig_x(deg_300, 300); }
    }
    if ((trig_y(deg_320, 320) < 25) and (trig_y(deg_320, 320) != 0)) {
        if ((trig_x(deg_320, 320) < left_dub) and (trig_x(deg_320, 320) != 0)) { left_dub = trig_x(deg_320, 320); }
    }

}

void scan(){

  turn_to_angle(0); delay(100);
  deg_0 = distance_average();
  
  turn_to_angle(20); delay(100);
  deg_20 = distance_average();
  
  turn_to_angle(40); delay(100);
  deg_40 = distance_average();
  
  turn_to_angle(60); delay(100);
  deg_60 = distance_average();
  
  turn_to_angle(80); delay(100);
  deg_80 = distance_average();
  
  turn_to_angle(100); delay(100);
  deg_100 = distance_average();
  
  turn_to_angle(120); delay(100);
  deg_120 = distance_average();
  
  turn_to_angle(140); delay(100);
  deg_140 = distance_average();
  
  turn_to_angle(160); delay(100);
  deg_160 = distance_average();
  
  turn_to_angle(180); delay(100);
  deg_180 = distance_average();
  
  turn_to_angle(200); delay(100);
  deg_200 = distance_average();
  
  turn_to_angle(220); delay(100);
  deg_220 = distance_average();
  
  turn_to_angle(240); delay(100);
  deg_240 = distance_average();
  
  turn_to_angle(260); delay(100);
  deg_260 = distance_average();
  
  turn_to_angle(280); delay(100);
  deg_280 = distance_average();
  
  turn_to_angle(300); delay(100);
  deg_300 = distance_average();
  
  turn_to_angle(320); delay(100);
  deg_320 = distance_average();
  
  turn_to_angle(340); delay(100);
  deg_340 = distance_average();
  
  turn_to_angle(0);
  left_stop();
  right_stop();

  eval_right_dub();
  eval_left_dub();
  eval_up_dub();
  eval_down_dub();
}

void move_by_encoders(int distance){
  long pos = 0;
  double angle = getHeading();
  angle = conversion(angle);
  robot_x += cos(angle)*distance;
  robot_y += sin(angle)*distance;
  myEnc.write(0);
  
  //distance is in mm
  drive_left_motor(0.1);
  drive_right_motor(0.1);
  while(abs(pos) < 1.6*distance){ pos = myEnc.read(); }
  left_stop();
  right_stop();
}
 
#endif
