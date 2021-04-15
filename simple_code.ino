#include "Adafruit_VL53L0X.h"
#include <math.h>
#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//Initialize the motor driver pins
const int IN1=8;
const int IN2=2;
const int ENA=9;

const int IN3=7;
const int IN4=3;
const int ENB=6;

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

//Motor functions
void Motor1_Forward(int Speed) 
{
     digitalWrite(IN1,HIGH); 
     digitalWrite(IN2,LOW);  
     analogWrite(ENA,Speed);
}
  
void Motor1_Backward(int Speed) 
{    
     digitalWrite(IN1,LOW); 
     digitalWrite(IN2,HIGH);  
     analogWrite(ENA,Speed);
}
void Motor1_Brake()      
{
     digitalWrite(IN1,LOW); 
     digitalWrite(IN2,LOW); 
}     
void Motor2_Forward(int Speed) 
{
     digitalWrite(IN3,HIGH); 
     digitalWrite(IN4,LOW);  
     analogWrite(ENB,Speed);
}
  
void Motor2_Backward(int Speed) 
{    
     digitalWrite(IN3,LOW); 
     digitalWrite(IN4,HIGH);  
     analogWrite(ENB,Speed);
}
void Motor2_Brake()
{
     digitalWrite(IN3,LOW); 
     digitalWrite(IN4,LOW); 
}

//method to turn to a certain degree specified --> 0->359 degrees, 0 deg is north
void turn(int turn){
  knob1.write(0);
  knob2.write(0);
    
  //turn right
  int angle_to_turn = turn-robot_angle;
  if (angle_to_turn < 0){
    while(abs(knob1.read()) < abs(angle_to_turn)){
      Motor1_Backward(100);
      Motor2_Forward(100);
      delay(1);
    }
   Motor1_Brake();
   Motor2_Brake();
  }
   
 if (angle_to_turn > 0){
  while(abs(knob1.read()) < abs(angle_to_turn)){
    Motor1_Forward(100);
    Motor2_Backward(100);
    delay(1);
  }
  Motor1_Brake();
  Motor2_Brake();
 }
 robot_angle = turn;
}

//method to move a certain distance specified
void move(int distance_to_move){
  knob1.write(0);
  knob2.write(0);
  
  double angle = double(distance_to_move)/0.881389;
  while((knob2.read()) < angle){
    Motor1_Forward(100);
    Motor2_Forward(100);
  }
}


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
  //Encoder stuff
  long newEnc1;
  long newEnc2;
  newEnc1 = knob1.read();
  newEnc1 = knob2.read();


  //need to go +X
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

  if(robot_x == end_x){
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
      
    }
      
    }
}      
