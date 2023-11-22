#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ESP32";
const char* password = "12345678";

WiFiServer webServer(80);
 
const char* apiURL = "https://project1-bd1a9-default-rtdb.firebaseio.com/Anime.json";
String header;
 
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;


void responsePage1(WiFiClient webClient){
    String html = {
    "<title>Page1</title>"
    "<body>"
        "<h1>Heading1</h1>"
        "<p>This is page 1</p>"
        "<ol>"
            "<li>Number1</li>"
            "<li>Number2</li>"
            "<li>Number3</li>"
        "</ol>"
        "<a href = \"/page2\">Next Page</a>"
        "<p>Textbox<input type=\"text\" name=\"txt1\" /></p>"
        "<button id=\"btn1\">Submit</button>"
    "</body>"
    };
    webClient.println(html);
}

void responsePage2(WiFiClient webClient){
    String html = {
    // "HTTP/1.1 200 OK"
    // "Content-type:text/html"
    // "Connection:close"
    "<title>Page2</title>"
    "<body>"
        "<h1>Heading2</h1>"
        "<p>This is page 2</p>"
        "<ul>"
            "<li>Number1</li>"
            "<li>Number2</li>"
            "<li>Number3</li>"
        "</ul>"
        "<a href = \"/\">Previous Page</a>"
        "<p>Textbox<input type=\"text\" name=\"txt1\" /></p>"
        "<button id=\"btn1\">Submit</button>"
    "</body>"
    };
    webClient.println(html);
}

String httpGETRequest(const char* serverName){
  HTTPClient http;
  http.begin(serverName);
  int responseCode = http.GET();
  String responseString = "{}";
  if (responseCode > 0) {
      responseString = http.getString();
      Serial.println(responseCode);
    }
  else {
    Serial.println("Error on HTTP GET request");
    Serial.println(responseCode);
    }
  http.end();
  return responseString;
}


void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP_STA);
  Serial.println("Setting AP mode");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP(); //mặc định là 192.168.4.1
  Serial.print("AP IP address: ");
  Serial.println(IP);
  /* start SmartConfig */
  WiFi.beginSmartConfig();

  /* Wait for SmartConfig packet from mobile */
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("SmartConfig done.");
 
  /* Wait for WiFi to connect to AP */
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
    String responseString= httpGETRequest(apiURL);                   
    Serial.println(responseString);
    delay(5000);
    
}



// void loop() {
//   WiFiClient webClient = webServer.available();
//   if(webClient)
//   {
//     currentTime = millis();
//     previousTime = currentTime;
//     Serial.println("New web Client");
//     String cur_line="";
//     while(webClient.connected() && currentTime - previousTime <= timeoutTime)
//     {
//         currentTime = millis();
//         if(webClient.available()){
//             char cur_char = webClient.read();
//             header += cur_char;
//             if(cur_char == '\n'){
//                 if (cur_line.length()==0){
//                     // Serial.println("This is the time");
//                     if(header.indexOf("GET /page2")>-1){
//                         responsePage2(webClient);
//                         // Serial.println("responsepage2");
//                         break;
//                         }
//                     else if(header.indexOf("GET / HTTP")>-1){
//                         responsePage1(webClient);
//                         // Serial.println("responsepage1");
//                         break;
//                         }
//                     }
//                 else{cur_line="";}
//                 }
//             else if(cur_char!='\r'){cur_line+=cur_char;}
//             }
//     }
//     Serial.println("This is header:");
//     Serial.println(header);
//     Serial.println();
//     header = "";
//     webClient.stop();
//     Serial.println("Client disconnected.");
//     Serial.println("");
//     }
// }


