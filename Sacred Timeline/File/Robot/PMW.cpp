#include<Arduino.h>
#include<Wire.h>
#include<Adafruit_PWMServoDriver.h>

#define SERVOMIN 50
#define SERVOMAX 500

Adafruit_PWMServoDriver pmw=Adafruit_PWMServoDriver();
uint8_t servo1=1;

uint8_t number1=75;
int number2=550;
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
            number3=map(number2,0,180,75,575);
            Serial.print("sernum:");
            Serial.println(number1);
            Serial.print("degree");
            Serial.println(number2);
            Serial.print("inputvalue:");
            Serial.println(number3);
            pmw.setPWM(number1,0,number3);
            delay(2000);
        }


    // pmw.setPWM(11,0,number1);
    // delay(2000);
    // pmw.setPWM(11,0,number2);
    // delay(3000);

    
    // pmw.setPWM(servo1,0,SERVOMAX);
    // delay(2000);
}