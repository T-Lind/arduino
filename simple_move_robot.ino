#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//determine the pulse length in microseconds from a percent power(+ is forward,, - is backward)
int pus(double power){
  if(power > 0){
    return (990*power + 1510); }//something like 0.50, meaning 50% power forwads
  if(power == 0){
    return 1500; }//0 power, meaning STOP
  else{
    return (-990*abs(power) + 1490); }//some negative power is the only other option, so it generates a power in the negative band with the absolute value of the power.
}

int conversion(double power){
  double pus = 0;//create variable pulse, in microseconds(us)
  int pticks = 0;//create pticks variable, in ticks out of 4096.
  pus = pus(power);//use the pus function to find the proportial pulse length in us from a percent
  phz = 1000000/pus//convert to hertz
  pticks = int((4096*phz)/1600)//convert hertz to ticks
  return pticks;
}


void setup() {
  // put your setup code here, to run once:
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency  
  Wire.setClock(400000);
  
  pwm.setPWM(4,0,conversion(0.50));//start motor at 50% power forward
  delay(1000)
  pwm.setPWM(4,0,4096);//stop motor
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
