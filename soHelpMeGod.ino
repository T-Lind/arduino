double deg0 = 0;
double deg20 = 0;
double deg40 = 0;
double deg60 = 0;
double deg80 = 0;
double deg100 = 0;
double deg120 = 0;
double deg140 = 0;
double deg160 = 0;
double deg180 = 0;
double deg200 = 0;
double deg220 = 0;
double deg240 = 0;
double deg260 = 0;
double deg280 = 0;
double deg300 = 0;
double deg320 = 0;
double deg340 = 0;

int up_dub = 700;   //Values that determine how far the vehicle can travel 
int down_dub = 700; //in any certain direction without being blocked.
int left_dub = 700; //if these values are less than 30, then the direction
int right_dub = 700;//is deemed completely blocked.

#include "pathfinding_methods.h"

#include "Adafruit_VL53L0X.h"
#include <math.h>
#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);

//Initialize the motor driver pins
const int IN1=13;
const int IN2=0;
const int ENA=12;

const int IN3=9;
const int IN4=10;
const int ENB=11;

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)

uint8_t servonum = 0;

//Initialize the encoder values
long encoder1_pos = -999;
long encoder2_pos = -999;
Encoder knob1(14,15);
Encoder knob2(16,17);

//Initialize the location values
int robot_x = 0;
int robot_y = 0;
int robot_angle = 0;//0 by default

int target_x = 100;//Some number
int target_y = 100;//Some number

int end_x = target_x - 382; //originally -38, +4
int end_y = target_y;

//Servo settings
void pulselength(int degrees)
{
  pulselength = map(degrees, 0, 180, SERVOMIN, SERVOMAX);
  return pulselength;
}
//convert from deg to radians on an equivalent spot on a unit circle
int conversion(int angle){
  if (angle < 90){
    int radians = ((-angle + 90)*3.14159)/180
  } else{
    int radians = ((-angle + 450)*3.14159)/180
  }
  return radians;
}


//method to turn to a certain degree specified --> 0->359 degrees, 0 deg is north
void setup() {
  Serial.begin(9600);

  //servo driver
  pwm.begin();
  pwm.setPWMFreq(1600);//maximum 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
     
  pinMode(IN4, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);

  //Set encoder values to 0 degrees
  knob1.write(0);
  knob2.write(0);
}

void loop() {
  boolean startAgain = false;
  //Encoder stuff
  long newEnc1;
  long newEnc2;
  newEnc1 = knob1.read();
  newEnc1 = knob2.read();
  //scan and get the distances until blocked
  int original_robot_angle = robot_angle;
  int vals[4] = pathfinding_methods scan();
  int up_dub = vals[0];
  int right_dub = vals[1];
  int down_dub = vals[2];
  int left_dub = vals[3];

  //thicc algorithm
  if(((target_x > robot_x)) && (startAgain = false)){
    if((right_dub > 300)or(right_dub = 0)){
      if(orignal_robot_angle != 270){
        pathfinding_methods turn(90);
        if(right_dub = 0){//no obstacle is seen which obscures
          pathfinding_methods move(700);
        }
        else if((robot_x + right_dub) < target_x){
          pathfinding_methods move(right_dub - 300);
        }else{
          pathfinding_methods move(target_x - robot_x);
        }
      }else{
        if((up_dub > 300)or(up_dub = 0)){
            pathfinding_methods turn(0);
            if(up_dub = 0){//no obstacle is seen which obscures
              pathfinding_methods move(700);
            }
            else if((robot_y + up_dub) < target_y){
              pathfinding_methods move(up_dub - 300);
            }else{
              pathfinding_methods move(target_y - robot_y);
            }
        }
        else if(down_dub > 300){
            pathfinding_methods turn(180);
            if(down_dub = 0){
              pathfinding_methods move(700);
            }
            else if((robot_y - down_dub) > target_y){
              pathfinding_methods move(down_dub-300);
            }else{
              pathfinding_methods move(robot_y - target_y);
            }
        }else if((left_dub > 300) or(left_dub = 0)){
          pathfinding_methods turn(270);
          if(left_dub = 0){
            pathfinding_methods move(700);
          }
          else{
            pathfinding_methods move(left_dub - 300);
          }
        }
      }
    }else{//if +x is actually blocked
      if(!(((robot_y - 10) < target_y) && (target_y < (robot_y + 10))) && (robot_y < target_y) && ((up_dub > 300) or (up_dub = 0))){
        if(orginal_robot_angle != 180){
          pathfinding_methods turn(0);
          if(up_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y + up_dub) < target_y){
            pathfinding_methods move(up_dub-300);
          }else{
            pathfinding_methods move(target_y-robot_y);
          }
        }else{//angle is 180 deg
          if((down_dub > 300) or (down_dub = 0)){
            pathfinding_methods turn(180);

            if(down_dub = 0){
              pathfinding_methods move(700);
            }
            else if((robot_y - down_dub) > target_y){
              pathfinding_methods move(down_dub-300);
            }else{
              pathfinding_methods move(robot_y - target_y);
            }
          }
        }
      }
      else if(!(((robot_y - 10) < target_y) && (target_y < (robot_y + 10))) && (robot_y > target_y) && ((down_dub > 300) or (down_dub = 0))){
        if(original_robot_angle != 0){
          pathfinding_methods turn(180);
          if(down_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y - down_dub) > target_y){
            pathfinding_methods move(down_dub - 300);
          }else{
            pathfinding_methods move(robot_y - target_y);
          }
        }else{//angle is 0 deg
          if((up_dub > 300) or (up_dub = 0)){
            pathfinding_methods turn(0);
            if(up_dub = 0){
              pathfinding_methods move(700);
            }
            else if((robot_y + up_dub) < target_y){
              pathfinding_methods move(up_dub - 300);
            }else{
              pathfinding_methods move(target_y - robot_y);
            }
          }
        }
      }else if(((robot_y - 10) < target_y) && (target_y < (robot_y + 10))){
        if((up_dub > 300) or (up_dub = 0)){
          pathfinding_methods turn(0);
          if(up_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y + up_dub) < target_y){
            pathfinding_methods move(up_dub - 300);
          }else{
            pathfinding_methods move(target_y - robot_y);
          }
        }
        else if((down_dub > 300) or (down_dub = 0)){
          pathfinding_methods turn(180);
          if(down_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y - down_dub) > target_y){
            pathfinding_methods move(down_dub - 300);
          }else{
            pathfinding_methods move(robot_y - target_y);
          }          
        }else if((left_dub > 300) or (left_dub = 0)){
          pathfinding_methods turn(270);
          if(left_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_x - left_dub) > target_x){
            pathfinding_methods move(left_dub - 300);
          }else{
            pathfinding_methods move(robot_x - target_x);
          }
        }
      }
    }
    startAgain = true;
  } 
//if I need to decrease x and I'm not within an acceptable range:
  if(((target_x < robot_x)) && (startAgain = false)){
    if((left_dub > 300)or(left_dub = 0)){
      if(orignal_robot_angle != 90){
        pathfinding_methods turn(270);
        if(left_dub = 0){//no obstacle is seen which obscures
          pathfinding_methods move(700);
        }
        else if((robot_x - left_dub) > target_x){
          pathfinding_methods move(left_dub - 300);
        }else{
          pathfinding_methods move(robot_x - target_x);
        }
      }else{
        if((up_dub > 300)or(up_dub = 0)){
            pathfinding_methods turn(0);
            if(up_dub = 0){//no obstacle is seen which obscures
              pathfinding_methods move(700);
            }
            else if((robot_y + up_dub) < target_y){
              pathfinding_methods move(up_dub - 300);
            }else{
              pathfinding_methods move(target_y - robot_y);
            }
        }
        else if(down_dub > 300){
            pathfinding_methods turn(180);
            if(down_dub = 0){
              pathfinding_methods move(700);
            }
            else if((robot_y - down_dub) > target_y){
              pathfinding_methods move(down_dub-300);
            }else{
              pathfinding_methods move(robot_y - target_y);
            }
        }else if((right_dub > 300) or(right_dub = 0)){
          pathfinding_methods turn(90);
          if(right_dub = 0){
            pathfinding_methods move(700);
          }
          else{
            pathfinding_methods move(right_dub - 300);
          }
        }
      }
    }else{//if -x is actually blocked
      if(!(((robot_y - 10) < target_y) && (target_y < (robot_y + 10))) && (robot_y < target_y) && ((up_dub > 300) or (up_dub = 0))){
        if(orginal_robot_angle != 180){
          pathfinding_methods turn(0);
          if(up_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y + up_dub) < target_y){
            pathfinding_methods move(up_dub-300);
          }else{
            pathfinding_methods move(target_y-robot_y);
          }
        }else{//angle is 180 deg
          if((down_dub > 300) or (down_dub = 0)){
            pathfinding_methods turn(180);

            if(down_dub = 0){
              pathfinding_methods move(700);
            }
            else if((robot_y - down_dub) > target_y){
              pathfinding_methods move(down_dub-300);
            }else{
              pathfinding_methods move(robot_y - target_y);
            }
          }
        }
      }
      else if(!(((robot_y - 10) < target_y) && (target_y < (robot_y + 10))) && (robot_y > target_y) && ((down_dub > 300) or (down_dub = 0))){
        if(original_robot_angle != 0){
          pathfinding_methods turn(180);
          if(down_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y - down_dub) > target_y){
            pathfinding_methods move(down_dub - 300);
          }else{
            pathfinding_methods move(robot_y - target_y);
          }
        }else{//angle is 0 deg
          if((up_dub > 300) or (up_dub = 0)){
            pathfinding_methods turn(0);
            if(up_dub = 0){
              pathfinding_methods move(700);
            }
            else if((robot_y + up_dub) < target_y){
              pathfinding_methods move(up_dub - 300);
            }else{
              pathfinding_methods move(target_y - robot_y);
            }
          }
        }
      }else if(((robot_y - 10) < target_y) && (target_y < (robot_y + 10))){
        if((up_dub > 300) or (up_dub = 0)){
          pathfinding_methods turn(0);
          if(up_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y + up_dub) < target_y){
            pathfinding_methods move(up_dub - 300);
          }else{
            pathfinding_methods move(target_y - robot_y);
          }
        }
        else if((down_dub > 300) or (down_dub = 0)){
          pathfinding_methods turn(180);
          if(down_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y - down_dub) > target_y){
            pathfinding_methods move(down_dub - 300);
          }else{
            pathfinding_methods move(robot_y - target_y);
          }          
        }else if((right_dub > 300) or (right_dub = 0)){
          pathfinding_methods turn(90);
          if(right_dub = 0){
            pathfinding_methods move(700);
          }
          else{
            pathfinding_methods move(right_dub - 300);
          }
        }
      }
    }
    startAgain = true;
  }
  
  if(((target_y > robot_y)) && (startAgain = false)){
    if((up_dub > 300)or(up_dub = 0)){
      if(orignal_robot_angle != 180){
        pathfinding_methods turn(0);
        if(up_dub = 0){//no obstacle is seen which obscures
          pathfinding_methods move(700);
        }
        else if((robot_y + up_dub) < target_y){
          pathfinding_methods move(up_dub - 300);
        }else{
          pathfinding_methods move(target_y - robot_y);
        }
      }else{
        if((right_dub > 300)or(right_dub = 0)){
            pathfinding_methods turn(90);
            if(right_dub = 0){//no obstacle is seen which obscures
              pathfinding_methods move(700);
            }//stopped working here
            else if((robot_x + right_dub) < target_x){
              pathfinding_methods move(up_dub - 300);
            }else{
              pathfinding_methods move(target_y - robot_y);
            }
        }
        else if(down_dub > 300){
            pathfinding_methods turn(180);
            if(down_dub = 0){
              pathfinding_methods move(700);
            }
            else if((robot_y - down_dub) > target_y){
              pathfinding_methods move(down_dub-300);
            }else{
              pathfinding_methods move(robot_y - target_y);
            }
        }else if((left_dub > 300) or(left_dub = 0)){
          pathfinding_methods turn(270);
          if(left_dub = 0){
            pathfinding_methods move(700);
          }
          else{
            pathfinding_methods move(left_dub - 300);
          }
        }
      }
    }else{//if +x is actually blocked
      if(!(((robot_y - 10) < target_y) && (target_y < (robot_y + 10))) && (robot_y < target_y) && ((up_dub > 300) or (up_dub = 0))){
        if(orginal_robot_angle != 180){
          pathfinding_methods turn(0);
          if(up_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y + up_dub) < target_y){
            pathfinding_methods move(up_dub-300);
          }else{
            pathfinding_methods move(target_y-robot_y);
          }
        }else{//angle is 180 deg
          if((down_dub > 300) or (down_dub = 0)){
            pathfinding_methods turn(180);

            if(down_dub = 0){
              pathfinding_methods move(700);
            }
            else if((robot_y - down_dub) > target_y){
              pathfinding_methods move(down_dub-300);
            }else{
              pathfinding_methods move(robot_y - target_y);
            }
          }
        }
      }
      else if(!(((robot_y - 10) < target_y) && (target_y < (robot_y + 10))) && (robot_y > target_y) && ((down_dub > 300) or (down_dub = 0))){
        if(original_robot_angle != 0){
          pathfinding_methods turn(180);
          if(down_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y - down_dub) > target_y){
            pathfinding_methods move(down_dub - 300);
          }else{
            pathfinding_methods move(robot_y - target_y);
          }
        }else{//angle is 0 deg
          if((up_dub > 300) or (up_dub = 0)){
            pathfinding_methods turn(0);
            if(up_dub = 0){
              pathfinding_methods move(700);
            }
            else if((robot_y + up_dub) < target_y){
              pathfinding_methods move(up_dub - 300);
            }else{
              pathfinding_methods move(target_y - robot_y);
            }
          }
        }
      }else if(((robot_y - 10) < target_y) && (target_y < (robot_y + 10))){
        if((up_dub > 300) or (up_dub = 0)){
          pathfinding_methods turn(0);
          if(up_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y + up_dub) < target_y){
            pathfinding_methods move(up_dub - 300);
          }else{
            pathfinding_methods move(target_y - robot_y);
          }
        }
        else if((down_dub > 300) or (down_dub = 0)){
          pathfinding_methods turn(180);
          if(down_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_y - down_dub) > target_y){
            pathfinding_methods move(down_dub - 300);
          }else{
            pathfinding_methods move(robot_y - target_y);
          }          
        }else if((left_dub > 300) or (left_dub = 0)){
          pathfinding_methods turn(270);
          if(left_dub = 0){
            pathfinding_methods move(700);
          }
          else if((robot_x - left_dub) > target_x){
            pathfinding_methods move(left_dub - 300);
          }else{
            pathfinding_methods move(robot_x - target_x);
          }
        }
      }
    }
    startAgain = true;
  } 

  if((robot_x >= end_x-10) && (robot_x <= end_x-10)){//check that the robot is within a certian distance from the target.
    Serial.println("feature()");
    while(!Serial.available());
    String data = Serial.readStringUntil('\n');
    
    while(data == "no_line_detected()"){//search for the object
      turn(45);
      robot_angle += 45;
      feature();
      while(!Serial.available());
      data = Serial.readStringUntil('\n');
     
    }
    if(data == "target_x"){
      while(!Serial.available());
      data = Serial.readStringUntil('\n');
      end_x = data.toInt();
      }
      
    while(!Serial.available());  
    data = Serial.readStringUntil('\n');
    if(data == "target_y"){
      while(!Serial.available());
      data = Serial.readStringUntil('\n');
      end_y = data.toInt();
      }
    robot_x = 0;
    robot_y = 0;
    while(true){
      if(end_x > robot_x){
        turn(90);
        move(end_x-robot_x);
        robot_x = robot_x + (end_x-robot_x);
      }
      if(end_x < robot_x){
        ////move left
        turn(270);
        move(robot_x-end_x);
        robot_x = robot_x - (robot_x-end_x);
      }
      if(end_y > robot_y){
        //move up
        turn(0);
        move(end_y-robot_y);
        robot_y = robot_y + (end_y-robot_y);
      }
      if(end_y < robot_y){
        //move down
        turn(180);
        move(robot_y-end_y);
        robot_y = robot_y - (robot_y-end_y);
      }
      //move arm, claw
      pwm.setPWM(0,0, pulselength(0)); //moving arm
      delay(1000);
      pwm.setPWM(1,0, pulselength(0)); //moving finger
      delay(1000);
      pwm.setPWM(2,0,pulselength(0));//rotating wrist
      delay(1000);
      
      pwm.setPWM(1,0,pulselength(135));//closing finger
      delay(1000);
      pwm.setPWM(2,0,pulselength(90));//rotating wrist back to 90
      delay(1000);
      pwm.setPWM(0,0,pulselength(180));//rotate servo back to resting position
    }
  }
}      
