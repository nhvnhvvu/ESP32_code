#include <WiFi.h>

const char* ssid = "BC68 T2";
const char* password =  "0904465635";
const uint16_t port = 80;
const char * host = "192.168.1.32"; //ko co http://

//neu host la https:// thi se ko ket noi dc do ko co chung chi SSL


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
    String data = "GET /PHP/Localhost/Form.html HTTP/1.1";
    WiFiClient client;
    if (!client.connect(host, port)) {
        Serial.println("Connection to host failed");
        delay(1000);
        return;
    }
    Serial.println("Connected to server successful!");
    Serial.println("Send:");

    client.print(String("GET /PHP/Localhost/Form.html HTTP/1.1\r\n") +
              "Host: 192.168.1.32\r\n" +
              "Connection: close\r\n\r\n");
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          break;
        }
      }
    }
    // Read the response
    String response;
    while (client.available()) {
      response += client.readStringUntil('\n');
    }
    Serial.println(response);
    // client.stop();
    Serial.println("Client disconnected");
}
