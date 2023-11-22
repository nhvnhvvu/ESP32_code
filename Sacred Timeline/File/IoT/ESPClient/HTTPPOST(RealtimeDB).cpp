#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

 
const char* ssid = "BC68 T2";
const char* password = "0904465635";
 
const char* apiURL = "https://project1-bd1a9-default-rtdb.firebaseio.com/Anime.json";
String httpPOSTRequest(const char*,JSONVar);

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
 
void loop() {
    String dataString = "{\"Shiraishi\":{\"id\":6,\"value\":\"high\"}}";
    JSONVar dataJson=JSON.parse(dataString);
    String responseString= httpPOSTRequest(apiURL,dataJson);           
    JSONVar JSONvar=JSON.parse(responseString);
    Serial.println(JSONvar);
    

    
 


    delay(20000);
    
}

String httpPOSTRequest(const char* serverName, JSONVar data){
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type","application/json");

  int responseCode = http.POST(JSON.stringify(data));
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

