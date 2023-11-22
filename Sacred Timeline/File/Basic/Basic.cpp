#include <Arduino.h>
#include <EEPROM.h>

// int freq=5000;
// int channel=0;
// int resolution=8;
// int x=0;
// int pin=16;
// int touchPin=4;
// int touchValue=0;

String getChipIdHex()
{
  String ChipIdHex = String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  ChipIdHex += String((uint32_t)ESP.getEfuseMac(), HEX);
  return ChipIdHex;
}

String getChipIdString()
{
  String ChipIdHex = String((uint32_t)(ESP.getEfuseMac() >> 32)); //phep dich bytes
  ChipIdHex += String((uint32_t)ESP.getEfuseMac());
  return ChipIdHex;
}


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN,HIGH);
  // digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);


  // analogPWMsetup
  // ledcSetup(channel,freq,resolution);
  // ledcAttachPin(pin,channel);
}

// the loop function runs over and over again forever
void loop() {

  //get chip id
  Serial.println(getChipIdString());
  Serial.println(getChipIdHex());
  delay(1000);


  // pinTouch
  // touchValue=touchRead(touchPin);
  // Serial.println(touchValue);
  // delay(500);

  // analogVoltage
  // for (int cycle=0;cycle<=255;cycle++){
  //   dacWrite(25,cycle);
  //   delay(100);
  // }

  // analogPWM
  // for(int cycle;cycle<=255;cycle++){
  //   ledcWrite(2,cycle);
  //   Serial.println(cycle); 
  //   delay(15);


  } 





// 216682415016456
// 54a48ff23a08
// 216682415016456
// 54a48ff23a08