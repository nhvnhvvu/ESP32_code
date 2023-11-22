#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// webServer.onNotFound(function); //xu li khi ko thay dia chi dc yeu cau
// webServer.on("/page1",function);  //xu li khi yeu cau dia chi 192.1.4.1/page1
// webServer.begin(); //bat dau webServer
// webServer.handleClient(); //lang nghe client
// String uri() {}; //get uri
// HTTPMethod method() {}; //get http method
// String pathArg(unsigned int i); // get request path argument by number
// String arg(String name);        // get request argument value by name
// String arg(int i);              // get request argument value by number
// String argName(int i);          // get request argument name by number
// int args();                     // get arguments count
// bool hasArg(String name);       // check if argument exists
// String header(String name);     // get request header value by name
// String header(int i);           // get request header value by number
// String headerName(int i);       // get request header name by number
// int headers();                  // get header count
// bool hasHeader(String name);    // check if header exists

const char* ssid = "BC68 T2";
const char* password = "0904465635";

WebServer webServer(80);
 
 
void responsePage1();
void responsePage2();
void handleNotFound();


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
   

    webServer.on("/",responsePage1);
    webServer.on("/page2",responsePage2);
    webServer.onNotFound(handleNotFound);
    webServer.begin();



}
 
void loop() {
    webServer.handleClient();
}


void responsePage1(){
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
    webServer.send(200,"text/html",html);
}

void responsePage2(){
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
        "<a href = \"/page3\">Next Page</a>"
        "<p>Textbox<input type=\"text\" name=\"txt1\" /></p>"
        "<button id=\"btn1\">Submit</button>"
    "</body>"
    };
    webServer.send(200,"text/html",html);

}

void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += webServer.uri();
    message += "\nMethod: ";
    message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += webServer.args();
    message += "\n";
    for (uint8_t i = 0; i < webServer.args(); i++) {
        message += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
    }
    webServer.send(404, "text/plain", message);
}