#include <Encoder.h>

Encoder knob(18,19);
void setup() {
  Serial.begin(9600);
  Serial.println("Encoder test! ");

}
long enc_pos = -999;

void loop() {
  long newEnc;
  newEnc = knob.read();
  if(newEnc != enc_pos){
    Serial.println(newEnc);
    enc_pos = newEnc;
  }
  if(Serial.available()){
    Serial.read();
    Serial.println("reset knobs to 0");
    knob.write(0);
  }
}
