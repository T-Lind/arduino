#include "robot.h"

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
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
  Serial.println(up_dub);
  Serial.println(right_dub);
  Serial.println(down_dub);
  Serial.println(left_dub);

}
void loop() {
}
