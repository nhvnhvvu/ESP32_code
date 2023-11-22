#include<Arduino.h>
#include<PubSubClient.h>
#include <WiFi.h>
// #include <HTTPClient.h>

#define BUTTON 34
#define RELAY 13
 
const char* ssid = "BC68 T2";
const char* password = "0904465635";

const char* mqtt_server = "192.168.110.208";

WiFiClient espClient;
PubSubClient client(espClient); //WiFiClient or EthernetClient (partial set, must set server and call back below)


int alert_number=0;
unsigned long alert_time = 0;

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

  if (String(topic) == "device1/status") { //name of the topic
    alert_number++;
    alert_time=millis();
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  pinMode(BUTTON,INPUT_PULLDOWN);
  pinMode(RELAY,OUTPUT);
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


}

void loop(){
  // Serial.println(digitalRead(BUTTON));
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      if (client.connect("Duong's ESP")) {
        Serial.println("connected");
        client.subscribe("device1/status");
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
  if(alert_number>2){
    digitalWrite(2,HIGH);
    digitalWrite(RELAY,LOW);
    alert_time = millis();
  }
  if(millis()-alert_time>5000){
    alert_number=0;
    // digitalWrite(2,HIGH);
    // digitalWrite(13,HIGH);
  }
  if(analogRead(BUTTON)==4095){
    delay(50);
    if (analogRead(BUTTON)==4095){
      delay(50);
        if(analogRead(BUTTON)==4095){
        Serial.println(number);
        number++;
        digitalWrite(RELAY, HIGH);
        alert_number=0;
        }
      }
    }
}


