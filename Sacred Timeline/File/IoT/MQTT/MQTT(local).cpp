#include<Arduino.h>
#include<PubSubClient.h>
#include <WiFi.h>
// #include <HTTPClient.h>

 
const char* ssid = "BC68 T2";
const char* password = "0904465635";

const char* mqtt_server = "192.168.1.32";

WiFiClient espClient;
PubSubClient client(espClient); //WiFiClient or EthernetClient (partial set, must set server and call back below)
// client.setServer(mqtt_server, 1883); //address of the server and the port connected to
// client.setCallback(callback); //callback function when new message is received

// client.connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage]) : connect to the server
  // clientID const char[] - the client ID to use when connecting to the server
  // Credentials - (optional)
  // username const char[] - the username to use. If NULL, no username or password is used
  // password const char[] - the password to use. If NULL, no password is used
  // Will - (optional) //WILL MESSAGE WILL BE SENT TO THE SERVER WHEN CLIENT DISCONNECT
  // willTopic const char[] - the topic to be used by the will message
  // willQoS int: 0,1 or 2 - the quality of service to be used by the will message
  // willRetain boolean - whether the will should be published with the retain flag
  // willMessage const char[] - the payload of the will message
// client.disconnect();

// client.publish (topic, payload, [length], [retained]) : Publish message to topic
  // topic const char[] - the topic to publish to
  // payload const char[], byte[] - the message to publish
  // length unsigned int (optional) - the length of the payload. Required if payload is a byte[]

// client.subscribe (topic, [qos]) : subscribe to topic
  // qos int: 0 or 1 only (optional) - the qos to subscribe at

// client.unsubscribe (topic) : unsubcribe topic
// client.loop(); should be called regularly to allow the client to process incoming messages and maintain its connection to the server
// client.connected(); check if client is connected

// client.state () current state of the client
  // Returns
  // -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
  // -3 : MQTT_CONNECTION_LOST - the network connection was broken
  // -2 : MQTT_CONNECT_FAILED - the network connection failed
  // -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
  // 0 : MQTT_CONNECTED - the client is connected
  // 1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
  // 2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
  // 3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
  // 4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
  // 5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect


long lastMsg=0;
int value=0;

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

  if (String(topic) == "esp32/output") { //name of the topic
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
  }
}

void setup(){
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
      if (client.connect("Duong's ESP")) {
        Serial.println("connected");
        client.subscribe("esp32/output");
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

  long now = millis();

  if (now - lastMsg > 2000) {
    lastMsg = now;
    value+=1;
    client.publish("esp/channel1",String(value).c_str());
  }
}


