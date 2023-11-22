#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <sstream>

#define BLUE_PIN 17
#define GREEN_PIN 18
#define RED_PIN 19
#define SERVO_PIN 32

#define DC_PIN_1 23


const char* ssid = "Redmi";
const char* password = "12345678";

// IPAddress local_IP(192, 168, 1, 184);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);
// IPAddress primaryDNS(192, 168, 1, 1);

IPAddress local_IP(192,168,226,184);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192,168,226,189);
IPAddress primaryDNS(192,168,226,189);

WiFiServer webServer(80);
 
String header="";

 
unsigned long currentTime = 0;
unsigned long previousTime = 0; 
const long timeoutTime = 10000;
String data;


Servo myservo;
int servo,bvalue,gvalue,rvalue;
int freq=5000;
int resolution=8;
int bchannel=2,gchannel=4,rchannel=7;




void setup() {
    Serial.begin(9600);
    if (!WiFi.config(local_IP, gateway, subnet,primaryDNS)) {
        Serial.println("STA Failed to configure");
        }
    
    Serial.print("Connecting to wifi:");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.subnetMask());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.dnsIP());


    ledcSetup(bchannel,freq,resolution);
    ledcAttachPin(BLUE_PIN,bchannel);
    ledcSetup(rchannel,freq,resolution);
    ledcAttachPin(RED_PIN,rchannel);
    ledcSetup(gchannel,freq,resolution);
    ledcAttachPin(GREEN_PIN,gchannel);

    webServer.begin();
    myservo.attach(32);
    myservo.write(50);

}
void loop(){
    WiFiClient webClient = webServer.available();
    if(webClient){
        while(webClient.available()){
            char cur_char = webClient.read();
            header += cur_char;
            }
        if(header!=""){
            Serial.println("This is header:");
            Serial.println(header);
            if(header.indexOf("CONTINUE")>-1){
                while(webClient.connected()){
                    header = "";
                    while(webClient.available()){
                        char cur_char = webClient.read();
                        header += cur_char;
                        }
                    if(header.indexOf("DOOR")>-1){
                        servo=header.substring(4).toInt();
                        Serial.print("Door:");
                        if(servo>44&&servo<131){
                            Serial.println(servo);
                            myservo.write(servo);
                            }
                    }
                    else if(header.indexOf("LEDB")>-1){
                        bvalue=header.substring(4).toInt();
                        ledcWrite(bchannel,bvalue);
                        Serial.println(bvalue);
                    }
                    else if(header.indexOf("LEDG")>-1){
                        gvalue=header.substring(4).toInt();
                        ledcWrite(gchannel,gvalue);
                        Serial.println(gvalue);
                    }       
                    else if(header.indexOf("LEDR")>-1){
                        rvalue=header.substring(4).toInt();
                        ledcWrite(rchannel,rvalue);
                        Serial.println(rvalue);
                    }         
                    else if(header.indexOf("CUR")>-1){
                        Serial.println("CUR");
                        digitalWrite(DC_PIN_1,HIGH);
                        delay(1500);
                        digitalWrite(DC_PIN_1,LOW);
                    } 
                      
                }
            }
            header = "";
        }
        Serial.println("DISCONNECTED");
    }
    }
