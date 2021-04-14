#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int pus(double power, bool direction){
  if(direction == true){
    return (990*power + 1510);
  }
  else{
    return (-990*power + 1490);
  }
}

int conversion(double power, bool direction){
  double pus = 0;
  int pticks
  pus = pus(power,direction);
  phz = 1000000/pus
  pticks = int((4096*phz)/1600)
  return pticks;
}


void setup() {
  // put your setup code here, to run once:
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency  
  Wire.setClock(400000);
  
  pwm.setPWM(4,0,conversion(0.50,true));
  delay(1000)
  pwm.setPWM(4,0,4096);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
