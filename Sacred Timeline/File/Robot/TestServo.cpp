#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN1 75
#define SERVOMAX1 575

#define SERVOMIN2 125 // 0,1
#define SERVOMAX2 650

#define SERVOMIN3 125 // 2,3
#define SERVOMAX3 600

int default_leg_state[8]={110,100,85,90,80,90,90,90};
int current_leg_state[8]={110,100,85,90,80,90,90,90};

Adafruit_PWMServoDriver pmw = Adafruit_PWMServoDriver();

int min_threshold(int i)
{
    if (i == 0 || i == 1)
    {
        return SERVOMIN2;
    }
    else if (i == 2 || i == 3)
    {
        return SERVOMIN3;
    }
    else
    {
        return SERVOMIN1;
    }
}

int max_threshold(int i)
{
    if (i == 0 || i == 1)
    {
        return SERVOMAX2;
    }
    else if (i == 2 || i == 3)
    {
        return SERVOMAX3;
    }
    else
    {
        return SERVOMAX1;
    }
}

void set_pwm(uint8_t sernum, int degree)
{
    int val = map(degree, 0, 180, min_threshold(sernum), max_threshold(sernum));
    pmw.setPWM(sernum, 0, val);
    current_leg_state[sernum] = degree;

}

void set_pwm_slow(uint8_t sernum,int cur_deg,int final_deg,int dtime=10){
    if (final_deg>cur_deg){
        for (int i=1;cur_deg+i<=final_deg;i++){
            int cur_val=map(cur_deg+i,0,180,min_threshold(sernum), max_threshold(sernum));
            pmw.setPWM(sernum,0,cur_val);
            delay(dtime);
        }
        current_leg_state[sernum]=final_deg;
        }
    else{
            for (int i=1;final_deg+i<=cur_deg;i++){
            int cur_val=map(cur_deg-i,0,180,min_threshold(sernum), max_threshold(sernum));
            pmw.setPWM(sernum,0,cur_val);
            delay(dtime);
        }
        current_leg_state[sernum]=final_deg;
    }
        

}

void set_leg_state(int arr[]){
    for (uint8_t i = 0; i < 8;i++){
        set_pwm(i,arr[i]);
    }
}

void left_step(){
    set_pwm_slow(3,current_leg_state[3],70);
    for(int i = 0; i < 41; i=i+2){
        int cur_val6=map(90+i,0,180,min_threshold(6), max_threshold(6));
        int cur_val4=map(80+i,0,180,min_threshold(4), max_threshold(4));
        int cur_val5=map(90+i,0,180,min_threshold(5), max_threshold(5));
        int cur_val7=map(90+i,0,180,min_threshold(7), max_threshold(7));
        int cur_val3=map(70+i/2,0,180,min_threshold(3), max_threshold(3));

        pmw.setPWM(3,0,cur_val3);
        pmw.setPWM(4,0,cur_val4);
        pmw.setPWM(5,0,cur_val5);
        pmw.setPWM(6,0,cur_val6);
        pmw.setPWM(7,0,cur_val7);
        delay(20);
    }
}



void left_leg_step1(){

    set_pwm(4,120);
    set_pwm(5,130);

    set_pwm(6,130);
    delay(200);
    set_pwm(7,130);
    delay(200);



}

void left_leg_step4(){

    set_pwm_slow(4,current_leg_state[4],120);
    set_pwm_slow(5,current_leg_state[5],130);

    set_pwm_slow(6,current_leg_state[6],130);

    set_pwm_slow(7,current_leg_state[7],130);




}
// void left_leg_step2(){
//     set_pwm(3,120);
//     delay(200);
//     set_pwm(1,90);
//     delay(200);


//     // set_pwm(2,60);

//     set_pwm(4,120);
//     set_pwm(5,130);

//     set_pwm(6,130);
//     delay(200);
//     set_pwm(7,130);
//     delay(200);
//     set_pwm(1,100);
//     set_pwm(3,90);


// }





void setup()
{
    Serial.begin(9600);
    Serial.println("Begin");

    pmw.begin();
    pmw.setPWMFreq(60);
    set_leg_state(default_leg_state);
    delay(2000);
}

void loop()
{
    if (Serial.available()){
        int number1 = Serial.readStringUntil(' ').toInt();
        int number2 = Serial.readStringUntil(' ').toInt();
        Serial.print("sernum:");
        Serial.println(number1);
        Serial.print("min:");
        Serial.println(number2);
        set_pwm_slow(number1,current_leg_state[number1],number2);
        delay(1000);
        }
    }

// {
//     if (Serial.available()){
//         number1 = Serial.readStringUntil(' ').toInt();
//         number2 = Serial.readStringUntil(' ').toInt();

//         Serial.print("sernum:");
//         Serial.println(number1);
//         Serial.print("deg:");
//         Serial.println(number2);
//         int val = map(number2, 0, 180, SERVOMIN2, SERVOMAX2);
//         Serial.print("number:");
//         Serial.println(val);
//         pmw.setPWM(number1, 0, val);
//         }
//     }
