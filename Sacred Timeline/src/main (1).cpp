#include <Arduino.h>
#include <Servo.h>

#define UART_BAUDRATE 9600
#define BUFFER_SIZE 64

#define LIMIT_SWITCH_PIN_1 9
#define LIMIT_SWITCH_PIN_2 10
#define LIMIT_SWITCH_PIN_3 11

#define GRIPPER_PIN 13

#define ENA_PIN 8
#define SPEED_DELAY_MS 500
#define SPEED_DELAY_MS_Z 500

#define STEP_PIN_1 2
#define DIR_PIN_1 5

#define STEP_PIN_2 3
#define DIR_PIN_2 6

#define STEP_PIN_3 4
#define DIR_PIN_3 7

unsigned long s1 = 0, s2 = 0, s3 = 0;
int state1 = 0, state2 = 0, state3 = 0;
int32_t cur1 = 0, cur2 = 0, cur3 = 0;
int32_t deg1 = 0, deg2 = 0, deg3 = 0;
bool stop1 = false, stop2 = false, stop3 = false;
int gripper = 0;

Servo myservo;


void rotate()
{
  while (deg1 || deg2 || deg3)
  {
    if (deg1)
    {
      if (micros() - s1 > SPEED_DELAY_MS)
      {
        state1 = 1 - state1;
        digitalWrite(STEP_PIN_1, state1);
        s1 = micros();
        deg1--;
      }
    }
    if (deg2)
    {
      if (micros() - s2 > SPEED_DELAY_MS)
      {
        state2 = 1 - state2;
        digitalWrite(STEP_PIN_2, state2);
        s2 = micros();
        deg2--;
      }
    }
    if (deg3)
    {
      if (micros() - s3 > SPEED_DELAY_MS_Z)
      {
        state3 = 1 - state3;
        digitalWrite(STEP_PIN_3, state3);
        s3 = micros();
        deg3--;
      }
    }
  }
  if (gripper){
    myservo.write(0);
    gripper=0;
  }
  else{
    myservo.write(60);
  }
  
}

void home(){
  stop1 = false;
  stop2 = false;
  stop3 = false;
  digitalWrite(DIR_PIN_2, LOW);
  digitalWrite(DIR_PIN_1, LOW);
  digitalWrite(DIR_PIN_3, LOW);
  // while (!digitalRead(LIMIT_SWITCH_PIN_1)||!digitalRead(LIMIT_SWITCH_PIN_2)||!digitalRead(LIMIT_SWITCH_PIN_3))
  while (!stop1||!stop2||!stop3)
  {
    if (!digitalRead(LIMIT_SWITCH_PIN_1)&&!stop1)
    {
      if (micros() - s1 > SPEED_DELAY_MS)
      {
        state1 = 1 - state1;
        digitalWrite(STEP_PIN_1, state1);
        s1 = micros();
      }
    }
    else{
      stop1=true;
      cur1=0;
    }
    if (!digitalRead(LIMIT_SWITCH_PIN_2)&&!stop2)
    {
      if (micros() - s2 > SPEED_DELAY_MS)
      {
        state2 = 1 - state2;
        digitalWrite(STEP_PIN_2, state2);
        s2 = micros();
      }
    }
    else{
      stop2=true;
      cur2=0;
    }
    if (!digitalRead(LIMIT_SWITCH_PIN_3)&&!stop3)
    {
      if (micros() - s3 > SPEED_DELAY_MS_Z)
      {
        state3 = 1 - state3;
        digitalWrite(STEP_PIN_3, state3);
        s3 = micros();
      }
    }
    else{
      stop3=true;
      cur3=0;
    }
  }
}

void setup()
{
  Serial.begin(UART_BAUDRATE);
  pinMode(STEP_PIN_1, OUTPUT);
  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(STEP_PIN_2, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);
  pinMode(STEP_PIN_3, OUTPUT);
  pinMode(DIR_PIN_3, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(GRIPPER_PIN, OUTPUT);
  pinMode(LIMIT_SWITCH_PIN_1, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_PIN_2, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_PIN_3, INPUT_PULLUP);

  digitalWrite(DIR_PIN_2, HIGH);
  digitalWrite(DIR_PIN_1, HIGH);
  digitalWrite(DIR_PIN_3, HIGH);
  digitalWrite(ENA_PIN, LOW);
  myservo.attach(GRIPPER_PIN);
  Serial.println("Start");
  //  rotate();
}

void loop()
{
  if (Serial.available())
  {
    String data = Serial.readStringUntil('\n');
    if (data.indexOf("X") > -1)
    {
      int32_t des1 = 0, des2 = 0, des3 = 0;
      sscanf(data.c_str(), "G%d X%d Y%d Z%d",&gripper, &des1, &des2, &des3); 
      deg1 = (des1 - cur1) * 2;
      deg2 = (des2 - cur2) * 2;
      deg3 = (des3 - cur3) * 2;
      cur1 = des1;
      cur2 = des2;
      cur3 = des3;
      Serial.println(deg1);
      Serial.println(deg2);
      Serial.println(deg3);

      if (deg1 < 0)
      {
        deg1 = -deg1;
        digitalWrite(DIR_PIN_1, LOW);
      }
      else
      {
        digitalWrite(DIR_PIN_1, HIGH);
      }
      if (deg2 < 0)
      {
        deg2 = -deg2;
        digitalWrite(DIR_PIN_2, LOW);
      }
      else
      {
        digitalWrite(DIR_PIN_2, HIGH);
      }
      if (deg3 < 0)
      {
        deg3 = -deg3;
        digitalWrite(DIR_PIN_3, LOW);
      }
      else
      {
        digitalWrite(DIR_PIN_3, HIGH);
      }
      rotate();
    }
    else if (data.indexOf("HOME") > -1)
    {
      home();
    }
  }
}

