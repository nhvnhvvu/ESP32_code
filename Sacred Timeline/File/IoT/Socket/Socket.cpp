#include <WiFi.h>
#include <Arduino.h>

IPAddress local_IP(192, 168, 1, 184);// Set your Static IP address
IPAddress gateway(192, 168, 1, 1);// Set your Gateway IP address
IPAddress subnet(255, 255, 0, 0);
const char* ssid = "BC68 T2";
const char* password = "0904465635";

String data;
int temp,humidity,lightness;

WiFiServer webServer(80);
 
String header="";

 
unsigned long currentTime = 0;
unsigned long previousTime = 0; 
const long timeoutTime = 2000;


void setup() {
  Serial.begin(9600);
 
  Serial.print("Connecting to wifi:");
  Serial.println(ssid);
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
    }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
   
  webServer.begin();
}
 
 void loop(){
    WiFiClient webClient = webServer.available();
    if(webClient){
      while(webClient.connected()){
        while(webClient.available()){
            char cur_char = webClient.read();
            header += cur_char;
            }
        if(header!=""){
            Serial.println("This is header:");
            Serial.println(header);
            if(header.indexOf("TEMP")>-1){
                temp+=1;
                data=String(temp);
                webClient.println(data);
              }
            if(header.indexOf("HUMID")>-1){
                humidity+=1;
                data=String(humidity);
                webClient.println(data);
              }
            if(header.indexOf("LIGHT")>-1){
                lightness+=10;
                data=String(lightness);
                webClient.println(data);
              }
            if(header.indexOf("DOOR")>-1){
                Serial.println("Door");
              }
            if(header.indexOf("CUR")>-1){
                Serial.println("Cur");
              }
            if(header.indexOf("LEDB")>-1){
                Serial.println("Ledb");
              }
            if(header.indexOf("LEDR")>-1){
                Serial.println("Ledr");
              }
            if(header.indexOf("LEDG")>-1){
                Serial.println("Ledg");
              }
            if(header.indexOf("SMS")>-1){
              Serial.println("sms");
            }
          }
        header = "";
        }
        Serial.println("Disconnected");
    }
  }


