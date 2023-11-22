#include<Arduino.h>
#include<PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>
// #include <HTTPClient.h>

#define BUTTON 34
#define RELAY 13
 
const char* ssid = "BC68 T2";
const char* password = "0904465635";

const char* mqtt_server = "Broker.hivemq.com";
// const char* mqtt_server = "mqtt.innoway.vn";
const char* topic = "3CSLab/AIday/new";
const char* payload = "ABCDEFGHIJKLMNOPQRSTUVWXY";
WiFiClient espClient;
PubSubClient client(espClient); //WiFiClient or EthernetClient (partial set, must set server and call back below)


int alert_number=0;
unsigned long alert_time = 0;
int mode = 0;
long lastMsg=0;
int value=0;
int number=0;

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

  if (String(topic) == "3CSLab/AIday/new") { //name of the topic
    Serial.println("HELLO");
    mode=2;
    // client.publish(topic,payload);
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
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
  // Serial.println("IP address: ");
  client.setServer(mqtt_server, 1883);
  digitalWrite(2,LOW);
  digitalWrite(13,HIGH);
  client.setCallback(callback);


    // client.publish(topic,payload);
}

void loop(){
  // Serial.println(digitalRead(BUTTON));
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      if (client.connect("Duong's ESP")) {
        Serial.println("connected");
        client.publish(topic,payload);
        client.subscribe("3CSLab/AIday/new");
      }
      else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }
  if(mode!=0){
    client.publish(topic,payload);
    mode=0;
  }
  client.loop();

}


