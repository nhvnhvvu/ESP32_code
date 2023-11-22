#include <Arduino.h>
#include <WiFi.h>


 
const char* ssid = "BC68 T2";
const char* password = "0904465635";
WiFiServer webServer(80);
 
 
String header;
 
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;
void responsePage1(WiFiClient);
void responsePage2(WiFiClient);


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
   
  webServer.begin();
}
 
void loop() {
  WiFiClient webClient = webServer.available();
  if(webClient)
  {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New web Client");
    String cur_line="";
    while(webClient.connected() && currentTime - previousTime <= timeoutTime)
    {
        currentTime = millis();
        if(webClient.available()){
            char cur_char = webClient.read();
            header += cur_char;
            if(cur_char == '\n'){
                if (cur_line.length()==0){
                    // Serial.println("This is the time");
                    if(header.indexOf("GET /page2")>-1){
                        responsePage2(webClient);
                        // Serial.println("responsepage2");
                        break;
                        }
                    else if(header.indexOf("GET / HTTP")>-1){
                        responsePage1(webClient);
                        // Serial.println("responsepage1");
                        break;
                        }
                    }
                else{cur_line="";}
                }
            else if(cur_char!='\r'){cur_line+=cur_char;}
            }
    }
    Serial.println("This is header:");
    Serial.println(header);
    Serial.println();
    header = "";
    webClient.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    }
}


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