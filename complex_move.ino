#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Encoder.h>

Encoder knob(14,15);
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

void move_distance(int distance){
  long enc_pos = -999;
  long newEnc;
  knob.write(0);
  newEnc = knob.read();
  
  
}

void setup() {
  // put your setup code here, to run once:
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(400);  // This is the maximum PWM frequency  
  Wire.setClock(400000);
  

}

void loop() {
  // put your main code here, to run repeatedly:
    //pwm.setPWM(4,0,conversion(.99));//start motor at 50% power forward
    pwm.setPWM(4,0,0);
    pwm.setPWM(5,0,0);
    delay(3000);
    pwm.setPWM(4,0,power(0.250));
    pwm.setPWM(5,0,power(-0.250));
    delay(1000);
    

}
