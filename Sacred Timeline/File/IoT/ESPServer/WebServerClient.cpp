#include <WiFi.h>

 
const char* ssid = "BC68 T2";
const char* password = "0904465635";
 
WiFiServer webServer(80);
 
void setup() {
  Serial.begin(9600);
 
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
   
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
        webClient.print("Data received");
        str="";
      }
 
      delay(10);
    }
 
    webClient.stop();
    Serial.println("Client disconnected");
 
  }
}