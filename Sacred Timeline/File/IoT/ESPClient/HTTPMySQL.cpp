#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
// #include <Arduino_JSON.h>

 
const char* ssid = "BC68 T2";
const char* password = "0904465635";


String apiURL = "http://192.168.1.32/PHP/ESP32.php";  // phai co http://

String httpGETRequest(String);
String httpPOSTRequest(String,String);

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
}
   
 
void loop(){
    if (Serial.available()){
        String UID = Serial.readString();
        String value1 = "value1";
        Serial.print("UID:");
        Serial.println(UID);
        // String query="?UID="+UID;
        // String payload = httpGETRequest(apiURL+query);

        String data="UID="+UID+"&value1="+value1;
        String payload = httpPOSTRequest(apiURL,data);
        Serial.println(payload);
        }



}

String httpGETRequest(String serverName){
  HTTPClient http;
  http.begin(serverName);

  int responseCode = http.GET();
  String responseString = "{}";
  if (responseCode > 0) {
      responseString = http.getString();
      Serial.println(responseCode);
    }
  else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(responseCode).c_str());
    }
  http.end();

  return responseString;

}

String httpPOSTRequest(String serverName, String data){
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int responseCode = http.POST(data);
  String responseString = "{}";
  if (responseCode > 0) {
      responseString = http.getString();
      Serial.println(responseCode);
    }
  else {
    Serial.println("Error on HTTP POST request");
    Serial.println(responseCode);
    }
  http.end();

  return responseString;

}


