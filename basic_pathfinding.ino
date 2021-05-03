#include "robot.h"

int target_x = 200;
int target_y = 200;

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
 /* 
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }*/
  if (!mag.begin()) {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }
  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(400);  // This is the maximum PWM frequency  
  Wire.setClock(400000);

  robot scan();

  while(((abs(robot_x) < abs(target_x) - 10) or (abs(robot_x) > abs(target_x) + 10)) and ((abs(robot_y) < abs(target_y) - 10) or (abs(robot_y) > abs(target_y) + 10))){
    Serial.println("in loop");
    if((robot_x < target_x) and (right_dub > 50)){
      robot turn_to_angle(90);
      robot move_by_encoders(right_dub-50);
    }
    if((robot_x > target_x) and (left_dub > 50)){
      robot turn_to_angle(270);
      robot move_by_encoders(left_dub-50);
    }
    if((robot_y < target_y) and (up_dub > 50)){
      robot turn_to_angle(0);
      robot move_by_encoders(right_dub-50);
    }
    if((robot_y > target_y) and (down_dub > 50)){
      robot turn_to_angle(180);
      robot move_by_encoders(right_dub-50);
    }
  }
}
void loop() {
  
}
