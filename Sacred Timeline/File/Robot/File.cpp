#include<Arduino.h>
#include<PubSubClient.h>
#include <WiFi.h>
const char* ssid = "BC68 T2";
const char* password = "0904465635";
const char* mqtt_server = "mqtt.innoway.vn";
WiFiClient espClient;
PubSubClient client(espClient);


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "messages/9084a7ab-9597-4004-ad9b-11e4f0670f89/status") { //name of the topic
    // Serial.print("Changing output to ");
    if(messageTemp.indexOf("on")>-1){
      Serial.println(messageTemp);
      // Serial.println("on");
      digitalWrite(2,1);
    }
    else if(messageTemp.indexOf("off")>-1){
      // Serial.println(messageTemp);
      // Serial.println("off");
      digitalWrite(2,0);
    }
  }
}


void setup(){
    Serial.begin(9600);
    pinMode(2,OUTPUT);
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
    

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    
}

void loop(){
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      if (client.connect("Duong's ESP","user1","qBCrWi8RzcQpWYjnalQqLlxaEHW3vrql")) {
        Serial.println("connected");
        client.subscribe("messages/9084a7ab-9597-4004-ad9b-11e4f0670f89/status");
      }
      else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }
  client.loop();
}


