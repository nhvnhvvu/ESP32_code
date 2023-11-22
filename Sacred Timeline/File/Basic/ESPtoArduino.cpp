#include<Arduino.h>


void setup(){
    Serial.begin(14400);
    Serial2.begin(9600);

}

void loop(){
    if(Serial2.available()){
        String data=Serial2.readString();
        Serial.println(data);
    }
}


