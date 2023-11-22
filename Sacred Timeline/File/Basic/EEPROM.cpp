#include<Arduino.h>
#include<EEPROM.h>

  //EEPROM 
// int address = 0;
// int value = 225; //from 0 to 255 byte
// int size = 512; //from 1 to 512 byte

// EEPROM.length();
// EEPROM.end();
// EEPROM.begin(size);
// EEPROM.write(address, value); //write value to address
// EEPROM.read(address); //read value from address
// EEPROM.commit(); //save the changes
String str;

void setup(){
    EEPROM.begin(512);
    delay(10);
    for (int i = 0; i < 32; ++i)
    {
        str += char(EEPROM.read(i));
    }
    Serial.println();
    Serial.print("String ");
    Serial.println(str);


}


void loop()
{
    if (Serial.available()){
        int number1 = Serial.readString().toInt();
        Serial.print("Number:");
        Serial.println(number1);

        delay(1000);
        }
    }





