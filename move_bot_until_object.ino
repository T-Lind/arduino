#include "Adafruit_VL53L0X.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int distance = 999;


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


void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(400);  // This is the maximum PWM frequency  
  Wire.setClock(400000);

  pwm.setPWM(4,0,power(0.2));
  pwm.setPWM(5,0,power(-0.2));

  while(distance > 200){
    VL53L0X_RangingMeasurementData_t measure;
      
    Serial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
      distance = measure.RangeMilliMeter;
    } else {
      Serial.println(" out of range ");
    }

  }

  pwm.setPWM(4,0,0);
  pwm.setPWM(5,0,0);
  
}


void loop() {
  }
 
