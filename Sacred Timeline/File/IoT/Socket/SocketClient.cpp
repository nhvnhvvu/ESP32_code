#include <WiFi.h>

const char* ssid = "BC68 T2";
const char* password =  "0904465635";

const uint16_t port = 8000;
const char * host = "192.168.1.32";

void setup()
{

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());

}

void loop()
{
    if(Serial.available()){
        String data=Serial.readString();
        WiFiClient client;
        if (!client.connect(host, port)) {
            Serial.println("Connection to host failed too");
            delay(1000);
            return;
        }
        Serial.println("Connected to server successful!");
        client.print(data);
        Serial.print("Send:");
        Serial.println(data);
        client.stop();
        Serial.println("Client disconnected");
}
}
