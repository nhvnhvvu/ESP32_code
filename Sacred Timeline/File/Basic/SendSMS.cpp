#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <sstream>


 
const char* ssid = "Redmi";
const char* password = "12345678";

const char* messagingServiceSID="MG0ab1394d599469f6d3de058a47fa3c59";
const char* phoneNumber="+84333387789";
const char* accountSID="AC6014d51c52106f78dfa3d85fdff7fd39";
const char* APIKey="f1d4e7475c7eb897559f89436b0c3834";

void sendSMS(const char*);

IPAddress local_IP(192,168,59,184);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192,168,59,44);
IPAddress primaryDNS(192,168,59,44);

void setup() {
  Serial.begin(9600);
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Sending Messages");
  sendSMS("Hello Duong");
  Serial.println("Sent Messages");
   
}
 
void loop() {
    

    
}


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
