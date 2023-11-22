#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN2 125 // 0,1
#define SERVOMAX2 650

int dls[10]={70,83,90,90,90,80,90,90,90,100};
int cls[10]={};

Adafruit_PWMServoDriver pmw = Adafruit_PWMServoDriver();

uint8_t number1=0;
int number2=0;
int number3=0;

void set_pwm(uint8_t sernum, int degree)
{
    int val = map(degree, 0, 180, SERVOMIN2, SERVOMAX2);
    pmw.setPWM(sernum, 0, val);
    cls[sernum] = degree;

}

void set_pwm_slow(uint8_t sernum1, int start1, int dis1, int dtime=1000){
    for (int i=1; i<21;i++){
        int cur_val1=map(start1+i*dis1/20,0,180,SERVOMIN2, SERVOMAX2);
        pmw.setPWM(sernum1,0,cur_val1);
        delay(dtime/20);
    }
}

void set_pwm_slow(uint8_t sernum1, uint8_t sernum2, int start1, int dis1, int start2, int dis2, int dtime=1000){
    for (int i=1; i<21;i++){
        int cur_val1=map(start1+i*dis1/20,0,180,SERVOMIN2, SERVOMAX2);
        int cur_val2=map(start2+i*dis2/20,0,180,SERVOMIN2, SERVOMAX2);
        pmw.setPWM(sernum1,0,cur_val1);
        pmw.setPWM(sernum2,0,cur_val2);
        delay(dtime/20);
    }
}

void set_leg_state(int arr[]){
    for (uint8_t i = 0; i <11 ;i++){
        set_pwm(i,arr[i]);
    }
}


void step1(){
    set_pwm_slow(2,cls[2],-30,6,cls[6],30);
    set_pwm_slow(3,cls[3],-30,7,cls[7],30);
}


void step2(){
    set_pwm_slow(0,cls[0],-15,8,cls[8],-15);
    set_pwm_slow(1,cls[1],15,9,cls[7],15);
}

void stand_up(){
    set_pwm_slow(8,cls[8],dls[8]-cls[8],9,cls[9],dls[9]-cls[9]);
    set_pwm_slow(0,cls[0],dls[0]-cls[0],1,cls[1],dls[1]-cls[1]);
    set_pwm_slow(6,cls[6],dls[6]-cls[6],7,cls[7],dls[7]-cls[7]);
    set_pwm_slow(5,cls[5],dls[5]-cls[5],4,cls[4],dls[4]-cls[4]);
    set_pwm_slow(3,cls[3],dls[3]-cls[3],2,cls[2],dls[2]-cls[2]);

}


// }




void setup()
{
    Serial.begin(9600);
    Serial.println("Begin");

    pmw.begin();
    pmw.setPWMFreq(60);
    set_leg_state(dls);
    delay(2000);
    Serial.print("Set default");
}

// void loop()
// {
//     if (Serial.available()){
//     {
//         uint8_t number1 = Serial.readStringUntil(' ').toInt();
//         if (number1==0){
//             Serial.println(number1);
//             stand_up();
//         }

//         else if(number1==1){
//             Serial.println(number1);
//             step1();
//         }
//         else if(number1==2){
//             Serial.println(number1);
//             step2();
//         }
//         // else if(number1==3){
//         //     Serial.println(number1);
//         //     left_step3();
//         // }
//         // else if(number1==4){
//         //     Serial.println(number1);
//         //     left_step4();
//         }
//         }
//     }

void loop(){
    if (Serial.available()){
        number1 = Serial.readStringUntil(' ').toInt();
        number2 = Serial.readStringUntil(' ').toInt();

        Serial.print("sernum:");
        Serial.println(number1);
        Serial.print("deg:");
        Serial.println(number2);
        set_pwm_slow(number1, cls[number1], number2);
        }
    }