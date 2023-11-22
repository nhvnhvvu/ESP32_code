#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <sstream>
#include "DHT.h"

#define LIGHT_PIN 35
#define TEMP_PIN 32
#define BUT_PIN 34


const char* messagingServiceSID="MG0ab1394d599469f6d3de058a47fa3c59";
const char* phoneNumber="+84333387789";
const char* accountSID="AC6014d51c52106f78dfa3d85fdff7fd39";
const char* APIKey="f1d4e7475c7eb897559f89436b0c3834";



// IPAddress local_IP(192, 168, 1, 184);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);
// IPAddress primaryDNS(192, 168, 1, 1);

IPAddress local_IP(192,168,216,184);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192,168,216,36);
IPAddress primaryDNS(192,168,216,36);
const char * host = "192.168.216.208";


WiFiServer webServer(80);
 
String header="";

 
String data;

DHT dht(32,DHT11);
int temp=0,humidity=0,lightness=0,val;
int button_state=1;
float currentTime = 0;
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



void send_data(String data){
  const uint16_t port = 8000;
  // const char * host = "192.168.1.32";
  
  WiFiClient client;
  if (!client.connect(host, port)) {
      Serial.println("Connection to host failed");
      return;
  }
  client.println(data);
}

const char* ssid = "Redmi";
const char* password = "12345678";


 
void setup() {
  Serial.begin(9600);
  dht.begin();
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
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

  webServer.begin();
  currentTime=millis();
}
 

 
void loop(){
    val=analogRead(34);
    
    if(val==0){
      button_state=0;
    }
    if(val==4095&&button_state==0){
      Serial.println("Press");
      send_data("BUTTON");
      button_state=4095;
    }
    temp=dht.readTemperature();
    humidity=dht.readHumidity();
    lightness=analogRead(35);
    // if(millis()-currentTime>2000){
    //   val=dht.readTemperature();
    //   if(temp!=val){
    //     data=String(temp);
    //     send_data("TEMP"+data);
    //     temp=val;
    //   }
    //   val=dht.readHumidity();
    //   if(humidity!=val){
    //     data=String(humidity);
    //     send_data("HUMID"+data);
    //     humidity=val;
    //   }
    //   val=analogRead(35);
    //   if(lightness!=val){
    //     data=String(lightness);
    //     send_data("LIGHT"+data);
    //     lightness=val;
    //   }
    //   currentTime=millis();
    // }

    
    WiFiClient webClient = webServer.available();

    if(webClient){
      //while(webClient.connected()){
        while(webClient.available()){
            char cur_char = webClient.read();
            header += cur_char;
            }
        if(header!=""){
          Serial.println("This is header:");
          Serial.println(header);
          if(header.indexOf("GET")>-1){
              data=String(temp);
              send_data("TEMP"+data);
              data=String(humidity);
              send_data("HUMID"+data);
              data=String(lightness);
              send_data("LIGHT"+data);
            }
          if(header.indexOf("LIGHTCON")>-1){
            while(webClient.connected()){
              lightness=analogRead(35);
              data=String(lightness);
              webClient.println("LIGHT"+data);
            }
          }
          if(header.indexOf("SMS")>-1){
            sendSMS("Co nguoi la trong nha");
          }
          Serial.println("DISCONNECTED");
          header = "";
        }
        }
        // webClient.stop();
        // Serial.println("Client disconnected.");
        // Serial.println("");
    //}
}

