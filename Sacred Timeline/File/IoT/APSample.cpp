#include <Arduino.h>
 
#include <WiFi.h>
//khai báo chân sử dụng led 
const int led1 = 2;
const int led2 = 27;
 
const char* ssid = "ESP32-Wifi-AP-Mode";
const char* password = "12345678";
//Tạo một web server tại cổng 80 - cổng mặc định cho web
WiFiServer webServer(80);
 
String led1Status = "OFF";
String led2Status = "OFF";
 
String header;
 
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
 
void setup() {
  Serial.begin(9600);
 
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  // Set outputs to LOW
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
 
  Serial.println("Setting AP mode");
  WiFi.softAP(ssid, password);
 
  IPAddress IP = WiFi.softAPIP(); //mặc định là 192.168.4.1
  Serial.print("AP IP address: ");
  Serial.println(IP);
  //khởi tạo webserver
  webServer.begin();
}
 
void loop() {
  WiFiClient webClient = webServer.available();
 
  if(webClient)
  {
    //khoi tao gia tri ban dau cho time
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New web Client");
    //biến lưu giá trị response
    String currentLine = "";
    //nếu có client connect và không quá thời gian time out
    while(webClient.connected() && currentTime - previousTime <= timeoutTime)
    {
      //đọc giá trị timer tại thời điểm hiện tại
      currentTime = millis();
      //nếu client còn kết nối
      if(webClient.available())
      {
        //đọc giá trị truyền từ client theo từng byte kiểu char
        char c = webClient.read();
        Serial.write(c);
        header += c; // lưu giá trị vào Header
        if(c == '\n') //Nếu đọc được kí tự xuống dòng (hết chuỗi truyền tới)
        {
          if (currentLine.length() == 0) 
          {
            // HTTP headers luôn luôn bắt đầu với code HTTP (ví d HTTP/1.1 200 OK)
            webClient.println("HTTP/1.1 200 OK");
            webClient.println("Content-type:text/html"); // sau đó là kiểu nội dụng mà client gửi tới, ví dụ này là html
            webClient.println("Connection: close"); // kiểu kết nối ở đây là close. Nghĩa là không giữ kết nối sau khi nhận bản tin
            webClient.println();
 
            // Response trang HTML 
            webClient.println("<!DOCTYPE html><html>");
            webClient.println("<head>Form head</head><hr />");
            webClient.println("<body>");
            webClient.println("<h1><a href=\"/page1\">Form body</a></h1>");
            webClient.println("<title>Form</a></title>");
            webClient.println("<form action=\"http://google.com\">");
            webClient.println("<p>Username:<input type=\"text\" name=\"username\" size=\"15\"maxlength=\"30\" /></p>");
            webClient.println("</form>");
            webClient.println("</body>");
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')   //nếu giá trị gửi tới khác xuống duòng
        {
          currentLine += c;     //lưu giá trị vào biến
        }
      }
    }
    // Xoá header để sử dụng cho lần tới
    header = "";
    // ngắt kết nối
    webClient.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
 
  }
}