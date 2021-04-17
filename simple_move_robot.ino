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
  double pius = 0;//create variable pulse, in microseconds(us)
  int pticks = 0;//create pticks variable, in ticks out of 4096.
  pius = pus(power);//use the pus function to find the proportial pulse length in us from a percent
  int phz = 1000000/pius;//convert to hertz
  pticks = int((4096*phz)/1600);//convert hertz to ticks
  return pticks;
}


void setup() {
  // put your setup code here, to run once:
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1000);  // This is the maximum PWM frequency  
  Wire.setClock(400000);
  

}

void loop() {
  // put your main code here, to run repeatedly:
    //pwm.setPWM(4,0,conversion(.99));//start motor at 50% power forward
    pwm.setPWM(4,0,1650);
    delay(1000);
    pwm.setPWM(4,4096,0);//stop motor
    delay(1000);
}
