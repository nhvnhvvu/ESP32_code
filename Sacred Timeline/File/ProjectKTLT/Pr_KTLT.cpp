#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <sstream>


const char* ap_ssid = "ESP32-Wifi-AP-Model";
const char* ap_password = "12345678";

const char* messagingServiceSID="MG0ab1394d599469f6d3de058a47fa3c59";
const char* phoneNumber="+84333387789";
const char* accountSID="AC6014d51c52106f78dfa3d85fdff7fd39";
const char* APIKey="f1d4e7475c7eb897559f89436b0c3834";
IPAddress local_IP(192, 168, 1, 184);// Set your Static IP address
IPAddress gateway(192, 168, 1, 1);// Set your Gateway IP address
IPAddress subnet(255, 255, 0, 0);

WiFiServer webServer(80);
 
String header="";
int state=1;
 
unsigned long currentTime = 0;
unsigned long previousTime = 0; 
const long timeoutTime = 10000;
String data;
int temp,humidity,lightness;

void sendSMS(const char* data){
  std::stringstream url;
  url<<"https://api.twilio.com/2010-04-01/Accounts/" <<accountSID<<"/Messages";
  std::stringstream urlBody;
  urlBody<<"MessagingServiceSid="<<messagingServiceSID<<"&To="<<phoneNumber<<"&Body="<<data;
  HTTPClient http;
  http.begin(url.str().c_str());
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.setAuthorization(accountSID, APIKey);
  int httpCode=http.POST(urlBody.str().c_str());
  String responseString = "{}";
  if (httpCode > 0) {
      responseString = http.getString();
      Serial.println(httpCode);
      // Serial.println(responseString);
    }
  else {
    Serial.println("Error on HTTP POST request");
    Serial.println(httpCode);
    // Serial.println(responseString);
    }
  http.end();
}


void setup(){
  Serial.begin(9600);
 
  WiFi.softAP(ap_ssid, ap_password);
 
  IPAddress IP = WiFi.softAPIP(); //mặc định là 192.168.4.1
  Serial.print("AP IP address: ");
  Serial.println(IP);
   
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
        }
        if(header!=""){
            Serial.println("This is header:");
            Serial.println(header);
            if(header.indexOf("SSID")>-1 && header.indexOf("PASS")>-1){
                String ssid_str = header.substring(header.indexOf("SSID")+4,header.indexOf("PASS"));
                const char* st_ssid=ssid_str.c_str();
                Serial.print("SSID: ");
                Serial.println(st_ssid);
                String pass_str = header.substring(header.indexOf("PASS")+4,header.indexOf("END"));
                const char* st_pass=pass_str.c_str();
                Serial.print("PASS: ");
                Serial.println(st_pass);

                //connect to wifi
                Serial.print("Connecting to wifi:");
                Serial.println(st_ssid);
                if (!WiFi.config(local_IP, gateway, subnet)) {
                    Serial.println("STA Failed to configure");
                    }
                WiFi.begin(st_ssid, st_pass);
                currentTime = millis();
                previousTime = currentTime;
                while (WiFi.status() != WL_CONNECTED && currentTime -previousTime<=timeoutTime){
                    currentTime = millis();delay(500);
                    Serial.print(".");
                    }
                if (WiFi.status() == WL_CONNECTED){
                    Serial.println();
                    Serial.println("WiFi connected.");
                    Serial.println("IP address: ");
                    Serial.println(WiFi.localIP());
                    webClient.print("OK");
                    WiFi.softAPdisconnect(true);
                    }
                else{
                    Serial.println();
                    Serial.println("Unable to connect");
                    webClient.print("FAIL");
                    }
            }
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
              sendSMS("Hello phone");
            }
            Serial.println(header);
        
            }
        header = "";
        webClient.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}

