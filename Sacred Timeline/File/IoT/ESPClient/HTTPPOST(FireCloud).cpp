#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

 
const char* ssid = "ssid";
const char* password = "password";
 
String apiURL = "https://firestore.googleapis.com/v1/projects/project1-bd1a9/databases/(default)/documents/";//url firecloud
String httpPOSTRequest(String,JSONVar);
void POSTData(String,String,String);

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
    int event=true;
    if(event){
    POSTData("document","field","value");
    delay(20000);
}
}


void POSTData(String document,String field,String field_value){
    String dataString = "{"
    "\"fields\": {\"" + field + "\": {"
    "\"stringValue\":\"" + field_value + "\"}"
    "}"
    "}" ;
    JSONVar dataJson=JSON.parse(dataString);
    String responseString= httpPOSTRequest(apiURL+document,dataJson);           
    JSONVar responseJson=JSON.parse(responseString);
    Serial.println(responseJson);
}

String httpPOSTRequest(String serverName, JSONVar data){
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type","application/json");
  int responseCode = http.POST(JSON.stringify(data));
  

  // If you need an HTTP request with a content type: x-www-form-urlencoded
  // http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";           
  // int httpResponseCode = http.POST(httpRequestData);
  
  // If you need an HTTP request with a content type: application/json,
  //http.addHeader("Content-Type", "application/json");
  //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

  // If you need an HTTP request with a content type: text/plain
  //http.addHeader("Content-Type", "text/plain");
  //int httpResponseCode = http.POST("Hello, World!");


  
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

