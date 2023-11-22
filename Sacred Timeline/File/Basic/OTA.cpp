#include <Arduino.h>
#include <HTTPUpdate.h>   //website: https://www.otadrive.com
 
const char* ssid = "BC68 T2";
const char* password = "0904465635";

uint32_t updateCounter = 0;
String version = "1.2";
String key = "09ae6427-03c0-4072-bd84-152cc200599b";  //get on web

void update_FOTA();
void setup()
{
    Serial.begin(9600);
    Serial.print("Connecting to Wifi:");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,password);
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.print("Connected to Wifi:");
    Serial.println(ssid);
    Serial.print("IP address:");
    IPAddress ip=WiFi.localIP();
    Serial.println(ip);
}
 
 
void loop()
{
  Serial.print("Ver: ");
  Serial.println(version);
  delay(500);
  if (WiFi.status() == WL_CONNECTED)
  {
    updateCounter++;
    if (updateCounter > 10)
    {
      updateCounter = 0;
      Serial.println("Check update");
      update_FOTA();
    }
  }
}
 
String getChipId()
{
  String ChipIdHex = String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  ChipIdHex += String((uint32_t)ESP.getEfuseMac(), HEX);
  return ChipIdHex;
}
 
void update_FOTA()
{
  String url = "http://otadrive.com/deviceapi/update?";
  url += "k=" + key;
  url += "&v=" + version;
  url += "&s=" + getChipId(); // định danh thiết bị trên Cloud
 
  WiFiClient client;
  httpUpdate.update(client, url, version);
}