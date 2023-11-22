#include<Arduino.h>
#include"DHT.h"


DHT dht(32,DHT11);
float temp,humidity,hic;
void setup(){
    Serial.begin(9600);
    Serial.println("Begin");
    dht.begin();
}


void loop(){
    temp=dht.readTemperature();
    humidity=dht.readHumidity();
    hic=dht.computeHeatIndex();
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(temp);
    Serial.print(F("°C  Heat index: "));
    Serial.print(hic);
    Serial.println(F("°C "));
    delay(1000);
}
  