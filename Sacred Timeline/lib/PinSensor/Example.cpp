#include<Arduino.h>
#include<PinSensor.h>

volatile boolean alert = false;
void lowPower() { alert = true; }

PinSensor gauge(0, lowPower); //alert pin




void setup()
{
    Serial.begin(9600);
    gauge.reset();
    delay(200); 
    gauge.setAlertThreshold(gauge.getSOC()-2.0);
    Serial.println(String("Alert Threshold is set to ") + 
                   gauge.getAlertThreshold() + '%');
}

void loop()
{
    Serial.print("SOC: ");
    Serial.print(gauge.getSOC()); 
    Serial.print("%, VCELL: ");
    Serial.print(gauge.getVoltage());
    Serial.println('V');
    if ( alert )
    {
        Serial.println("Beware, Low Power!");
        gauge.clearAlertInterrupt();
        alert = false;
        Serial.println("System operations are halted...");
        gauge.sleep();
        while ( true ){
            Serial.println("DMB");
            delay(2000);
        } ;
    }
    delay(2000);
}

