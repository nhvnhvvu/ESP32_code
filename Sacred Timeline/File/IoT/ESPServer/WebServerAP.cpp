#include <WiFi.h>

 
const char* ssid = "ESP32-Wifi-AP-Mode";
const char* password = "12345678";
 
WiFiServer webServer(80);
 
void setup() {
  Serial.begin(9600);
 
  WiFi.softAP(ssid, password);
 
  IPAddress IP = WiFi.softAPIP(); //mặc định là 192.168.4.1
  Serial.print("AP IP address: ");
  Serial.println(IP);
   
  webServer.begin();
}
 
void loop() {
 
  WiFiClient webClient = webServer.available();
  String str="";
  if (webClient) {
    webClient.print("Connected");
    while (webClient.connected()) {
 
      while (webClient.available()>0) {
        char c = webClient.read();
        str+=c;
      }
      if (str!=""){
        Serial.println(str);
        str="";
      }
 
      delay(10);
    }
 
    webClient.stop();
    Serial.println("Client disconnected");
 
  }
}