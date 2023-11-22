#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN2 125 // 0,1
#define SERVOMAX2 650

int dls[10]={70,81,90,85,90,90,90,90,90,85};
int cls[10]={};
int cur_val[10]={0,0,0,0,0,0,0,0,0,0};

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

void set_pwm_slow(uint8_t sernum1, int start1, int dis1, int dtime=2000){
    for (int i=1; i<21;i++){
        int cur_val1=map(start1+i*dis1/20,0,180,SERVOMIN2, SERVOMAX2);
        pmw.setPWM(sernum1,0,cur_val1);
        delay(dtime/20);
    }
}

// void set_pwm_slow(uint8_t sernum1,int start1,int dis1, uint8_t sernum2, int start2, int dis2, int dtime=1000){
//     for (int i=1; i<21;i++){
//         int cur_val1=map(start1+i*dis1/20,0,180,SERVOMIN2, SERVOMAX2);
//         int cur_val2=map(start2+i*dis2/20,0,180,SERVOMIN2, SERVOMAX2);
//         pmw.setPWM(sernum1,0,cur_val1);
//         pmw.setPWM(sernum2,0,cur_val2);
//         delay(dtime/20);
//     }
// }

void set_pwm_slow(int n,int serList[],int degList[], int dtime=3000){
    for (int i=1; i<21;i++){
        for (int j=0;j<n;j++){
            cur_val[serList[j]]=map(cls[serList[j]]+i*degList[serList[j]]/20,0,180,SERVOMIN2, SERVOMAX2);
            pmw.setPWM(serList[j],0,cur_val[serList[j]]);
        }
        delay(dtime/20);
    }
}

void set_leg_state(int arr[]){
    for (uint8_t i = 0; i <11 ;i++){
        set_pwm(i,arr[i]);
    }
}


void step6(){
    int arr[]={2,3,6,7};
    int arr2[]={-20,-20,-20,-20};
    set_pwm_slow(4,arr,arr2);
}


void step2(){
    int arr[]={0,1,8,9};
    int arr2[]={-20,20,20,-20};
    set_pwm_slow(4,arr,arr2); //banh hang loai 1
}

void step1(){
    int arr[]={0,1,8,9};
    int arr2[]={-20,20,-20,20};
    set_pwm_slow(4,arr,arr2); //banh hang loai 2
}

void step3(){
    int arr[]={0,1,9};
    int arr2[]={-20,-20,20};
    set_pwm_slow(3,arr,arr2); //nghieng hong phai
}

void step4(){
    int arr[]={0,1,8};
    int arr2[]={20,20,-20};
    set_pwm_slow(3,arr,arr2); //nghieng hong trai
}

void step5(){
    int arr[]={4,5,6,7};
    int arr2[]={-30,30,30,30};
    set_pwm_slow(4,arr,arr2); //quy
}

void test(){
    int arr[]={8,9};
    int arr2[]={20,20};
    set_pwm_slow(2,arr,arr2);
}


void stand_up(){
    int arr[]={9,8,7,6,5,4,3,2,1,0};
    int arr2[]={dls[9]-cls[9],dls[8]-cls[8],dls[7]-cls[7],dls[6]-cls[6],dls[5]-cls[5],dls[4]-cls[4],dls[3]-cls[3],dls[2]-cls[2],dls[1]-cls[1],dls[0]-cls[0]};
    set_pwm_slow(10,arr,arr2);
}







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

void loop()
{
    if (Serial.available())
    {
        uint8_t number1 = Serial.readStringUntil(' ').toInt();
        if (number1==0){
            Serial.println(number1);
            stand_up();
        }

        else if(number1==1){
            Serial.println(number1);
            step1();
        }
        else if(number1==2){
            Serial.println(number1);
            step2();
        }
        else if(number1==3){
            Serial.println(number1);
            step3();
        }
        else if(number1==4){
            Serial.println(number1);
            step4();
        }
        else if(number1==5){
            Serial.println(number1);
            step5();
        }
        else if(number1==6){
            Serial.println(number1);
            step6();
        }
        else if(number1==7){
            Serial.println(number1);
            test();
        }
        }
    }

// void loop(){
//     if (Serial.available()){
//         number1 = Serial.readStringUntil(' ').toInt();
//         number2 = Serial.readStringUntil(' ').toInt();

//         Serial.print("sernum:");
//         Serial.println(number1);
//         Serial.print("deg:");
//         Serial.println(number2);
//         // int val=map(number2,0,180,SERVOMIN2, SERVOMAX2);
//         // pmw.setPWM(number1,0,val);
//         set_pwm_slow(number1,cls[number1],number2);
//         }
//     }