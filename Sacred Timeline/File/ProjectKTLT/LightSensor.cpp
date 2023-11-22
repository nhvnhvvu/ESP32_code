#include<Arduino.h>



int light;
void setup(){
    Serial.begin(9600);
    Serial.println("Begin");
}


void loop(){
    light=analogRead(35);
    Serial.print(F("Light: "));
    Serial.println(light);
    delay(100);
}
  