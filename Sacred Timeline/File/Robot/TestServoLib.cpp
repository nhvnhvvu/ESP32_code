#include<ESP32Servo.h>
#include<Arduino.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  myservo.attach(32);
  myservo.write(50);

}

void loop() {
    if (Serial.available()){
        int number1 = Serial.readStringUntil(' ').toInt();
        if (number1 == 1){
          myservo.write(120);
          Serial.println(number1);
        }
        else{
          myservo.write(50);
          Serial.println(number1);
        }
}
}