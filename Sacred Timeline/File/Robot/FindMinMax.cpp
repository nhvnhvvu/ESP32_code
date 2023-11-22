#include<Arduino.h>
#include<Wire.h>
#include<Adafruit_PWMServoDriver.h>

// name less: 75-575
// Tiankong 125 650
// TowerPower 125 650
// name less under 125 600

Adafruit_PWMServoDriver pmw=Adafruit_PWMServoDriver();

uint8_t number1=0;
int number2=0;
int number3=0;

void setup(){
    Serial.begin(9600);
    Serial.println("Begin");
    pmw.begin();
    
    pmw.setPWMFreq(60);
}

void loop(){
    if (Serial.available()){
        number1 = Serial.readStringUntil(' ').toInt();
        number2 = Serial.readStringUntil(' ').toInt();
        number3 = Serial.readStringUntil(' ').toInt();
        Serial.print("sernum:");
        Serial.println(number1);
        Serial.print("min:");
        Serial.println(number2);
        Serial.print("max:");
        Serial.println(number3);
        pmw.setPWM(number1,0,number2);
        delay(2000);
        pmw.setPWM(number1,0,number3);
        delay(2000);
        }



}