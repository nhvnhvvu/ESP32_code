#include <Arduino.h>
#include <WiFi.h>


 
const char* ssid = "BC68 T2";
const char* password = "0904465635";
WiFiServer webServer(80);
 
 
String header;
 
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;
void responseGET1(WiFiClient);
void responseGET2(WiFiClient);
void responsePOST(WiFiClient);


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
    // Serial.println("New web Client");
    String prev_line="";
    String cur_line="";
    while(webClient.connected() && currentTime - previousTime <= timeoutTime)
    {
        currentTime = millis();
        if(webClient.available()){
            char cur_char = webClient.read();
            header += cur_char;
            if(cur_char == '\n'){
                if (cur_line.length()==0){
                    if(header.indexOf("GET / ")>-1){
                        responseGET1(webClient);
                        break;
                        }
                    else if(header.indexOf("GET /page2")>-1){
                        responseGET2(webClient);
                        break;
                        }
                    else{
                        webClient.println("HELLO");

                    }
                    // else if(header.indexOf("POST")>-1){
                    //     webClient.println("HTTP/1.1 200 OK");
                    //     break;
                    // }
                    }
                else{
                    prev_line=cur_line;
                    cur_line="";
                    }
                }
            else if(cur_char!='\r'){cur_line+=cur_char;}
            }
    }
    if(header!=""){
        Serial.println("This is header:");
        Serial.println(header);
    }
    if(header.indexOf("POST")>-1){
        Serial.print("String received:");
        Serial.println(cur_line);
        webClient.println("HTTP/1.1 200 OK");
        }
    header = "";
    webClient.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    }
}


void responseGET1(WiFiClient webClient){
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
    "<script>"
    "start();"
    "function start(){"
        "btnClick();"
    "}"
    "function btnClick(){"
        "var url = \"http://192.168.1.24\";"   //localIP
        "var but1 = document.getElementById(\"btn1\");"
        "but1.onclick = function(){"
            "var name = document.querySelector(\"input[name=txt1]\").value;"
            "document.querySelector(\"input[name=txt1]\").value= '';"
            "var object={"
                "method: \"POST\","
                "mode: \"no-cors\","
                "body: name,"
            "};"
            "fetch(url,object);"
        "}"
    "}"
    "</script>"
    };
    webClient.println("HTTP/1.1 200 OK");
    webClient.println("Content-type:text/html");
    webClient.println();
    webClient.println(html);
}


void responseGET2(WiFiClient webClient){
    String html = {
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
    "<script>"
    "start();"
    "function start(){"
        "btnClick();"
    "}"
    "function btnClick(){"
        "var url = \"http://192.168.1.24\";"
        "var but1 = document.getElementById(\"btn1\");"
        "but1.onclick = function(){"
            "var name = document.querySelector(\"input[name=txt1]\").value;"
            "document.querySelector(\"input[name=txt1]\").value= '';"
            "var object={"
                "method: \"POST\","
                "mode: \"no-cors\","
                "body: name,"
            "};"
            "fetch(url,object);"
        "}"
    "}"
    "</script>"
    };
    webClient.println("HTTP/1.1 200 OK");
    webClient.println("Content-type:text/html");
    webClient.println();
    webClient.println(html);
}

// void responsePOST(WiFiClient webClient){
//     String html={
//     "<title>Response</title>"
//     "<body>"
//         "<h1>HeadingResponese</h1>"
//         "<p>This is page response</p>"
//         "<a href = \"/\">Return to page 1</a>"
//         "<a href = \"/page2\">Return to page 2</a>"
//     "</body>"
//     };
//     webClient.println("HTTP/1.1 200 OK");
//     webClient.println("Content-type:text/html");
//     webClient.println();
//     webClient.println(html);
// }
