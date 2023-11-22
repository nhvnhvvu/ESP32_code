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

void left_step1(){
    set_pwm_slow(3,current_leg_state[3],50);
    delay(500);
    for(int i = 0; i < 41; i=i+2){
        int cur_val6=map(90+i,0,180,min_threshold(6), max_threshold(6));
        int cur_val4=map(80+i,0,180,min_threshold(4), max_threshold(4));
        int cur_val5=map(90+i,0,180,min_threshold(5), max_threshold(5));
        int cur_val7=map(90+i,0,180,min_threshold(7), max_threshold(7));
        int cur_val3=map(50+i,0,180,min_threshold(3), max_threshold(3));

        pmw.setPWM(3,0,cur_val3);
        pmw.setPWM(4,0,cur_val4);
        pmw.setPWM(5,0,cur_val5);
        pmw.setPWM(6,0,cur_val6);
        pmw.setPWM(7,0,cur_val7);
        delay(20);
    }
}

void left_step2(){
    set_pwm_slow(3,current_leg_state[3],50);
    delay(500);
    set_pwm_slow(1,current_leg_state[1],80);
    delay(500);
    for(int i = 0; i < 41; i=i+2){
        int cur_val6=map(90+i,0,180,min_threshold(6), max_threshold(6));
        int cur_val4=map(80+i,0,180,min_threshold(4), max_threshold(4));
        int cur_val5=map(90+i,0,180,min_threshold(5), max_threshold(5));
        int cur_val7=map(90+i,0,180,min_threshold(7), max_threshold(7));
        int cur_val3=map(50+i,0,180,min_threshold(3), max_threshold(3));
        int cur_val1=map(80+i/2,0,180,min_threshold(1), max_threshold(1));

        pmw.setPWM(3,0,cur_val3);
        pmw.setPWM(4,0,cur_val4);
        pmw.setPWM(5,0,cur_val5);
        pmw.setPWM(6,0,cur_val6);
        pmw.setPWM(7,0,cur_val7);
        pmw.setPWM(1,0,cur_val1);

        delay(20);
    }
}

void left_step3(){
    set_pwm_slow(3,current_leg_state[3],50);
    delay(500);
    set_pwm_slow(1,current_leg_state[1],80);
    delay(500);
    set_pwm_slow(4,current_leg_state[4],120);
    set_pwm_slow(5,current_leg_state[5],120);
    for(int i = 0; i < 41; i=i+2){
        int cur_val6=map(90+i,0,180,min_threshold(6), max_threshold(6));
        int cur_val7=map(90+i,0,180,min_threshold(7), max_threshold(7));
        int cur_val3=map(50+i,0,180,min_threshold(3), max_threshold(3));
        int cur_val1=map(80+i/2,0,180,min_threshold(1), max_threshold(1));

        pmw.setPWM(3,0,cur_val3);
        pmw.setPWM(6,0,cur_val6);
        pmw.setPWM(7,0,cur_val7);
        pmw.setPWM(1,0,cur_val1);

        delay(20);
    }
}

void left_step4(){
    set_pwm_slow(3,current_leg_state[3],50);
    delay(500);
    set_pwm_slow(1,current_leg_state[1],80);
    delay(500);
    set_pwm(4,120);
    set_pwm(5,120);
    for(int i = 0; i < 41; i=i+2){
        int cur_val6=map(90+i,0,180,min_threshold(6), max_threshold(6));
        int cur_val7=map(90+i,0,180,min_threshold(7), max_threshold(7));
        int cur_val3=map(50+i,0,180,min_threshold(3), max_threshold(3));
        int cur_val1=map(80+i/2,0,180,min_threshold(1), max_threshold(1));

        pmw.setPWM(3,0,cur_val3);
        pmw.setPWM(6,0,cur_val6);
        pmw.setPWM(7,0,cur_val7);
        pmw.setPWM(1,0,cur_val1);

        delay(20);
    }
}



// }




void setup()
{
    Serial.begin(9600);
    Serial.println("Begin");

    pmw.begin();
    pmw.setPWMFreq(60);
    set_leg_state(default_leg_state);
    delay(3000);
}

void loop()
{

    if (Serial.available())
    {
        uint8_t number1 = Serial.readStringUntil(' ').toInt();
        if (number1==0){
            Serial.println(number1);
            set_leg_state(default_leg_state);
        }
        // else if(number1==1){
        //     Serial.println(number1);
        //     left_leg_step1();
        // }

        // else if(number1==4){
        //     Serial.println(number1);
        //     left_leg_step4();
        // }
        else if(number1==1){
            Serial.println(number1);
            left_step1();
        }
        else if(number1==2){
            Serial.println(number1);
            left_step2();
        }
        else if(number1==3){
            Serial.println(number1);
            left_step3();
        }
        else if(number1==4){
            Serial.println(number1);
            left_step4();
        }
        }
    }


