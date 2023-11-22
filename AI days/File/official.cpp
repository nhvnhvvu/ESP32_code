#include <Adafruit_Fingerprint.h>
#include<PubSubClient.h>
#include <WiFi.h>
#include <Wire.h> 

#define mySerial Serial2

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int mode = 2;

const char* ssid = "BC68 T2";
const char* password = "0904465635";

const char* mqtt_server = "Broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "3CSLab/AIday/new") { //name of the topic
    mode = 1;
  }
}

void setup()  
{
  Serial.begin(9600);
  pinMode(2,OUTPUT);
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
  // Serial.println("IP address: ");
  client.setServer(mqtt_server, 1883);
  digitalWrite(2,HIGH);

  client.setCallback(callback);
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
}


void loop()
{
    if (!client.connected()) {
        while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("Duong's ESP")) {
            Serial.println("connected");
            client.subscribe("3CSLab/AIday/new");
        }
        else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
        }
    }
    client.loop();
    int p = -1;
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
        case FINGERPRINT_NOFINGER:
        break;
        case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
        case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
        default:
        Serial.println("Unknown error");
        break;
        }
    }
    if(mode==1){
        int id = finger.getTemplateCount()+1;
        p = finger.image2Tz(1);
        switch (p) {
            case FINGERPRINT_OK:
            Serial.println("Image converted");
            break;
            case FINGERPRINT_IMAGEMESS:
            Serial.println("Image too messy");
            break;
            case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            break;
            case FINGERPRINT_FEATUREFAIL:
            Serial.println("Could not find fingerprint features");
            break;
            case FINGERPRINT_INVALIDIMAGE:
            Serial.println("Could not find fingerprint features");
            break;
            default:
            Serial.println("Unknown error");
            break;
        }
        
        Serial.println("Remove finger");
        delay(2000);
        p = 0;
        while (p != FINGERPRINT_NOFINGER) {
            p = finger.getImage();
        }
        Serial.print("ID "); Serial.println(id);
        p = -1;
        Serial.println("Place same finger again");
        while (p != FINGERPRINT_OK) {
            p = finger.getImage();
            switch (p) {
            case FINGERPRINT_OK:
            Serial.println("Image taken");
            break;
            case FINGERPRINT_NOFINGER:
            break;
            case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            break;
            case FINGERPRINT_IMAGEFAIL:
            Serial.println("Imaging error");
            break;
            default:
            Serial.println("Unknown error");
            break;
            }
        }
        p = finger.image2Tz(2);
        switch (p) {
            case FINGERPRINT_OK:
            Serial.println("Image converted");
            break;
            case FINGERPRINT_IMAGEMESS:
            Serial.println("Image too messy");
            break;
            case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            break;
            case FINGERPRINT_FEATUREFAIL:
            Serial.println("Could not find fingerprint features");
            break;
            case FINGERPRINT_INVALIDIMAGE:
            Serial.println("Could not find fingerprint features");
            break;
            default:
            Serial.println("Unknown error");
            break;
        }
        Serial.print("Creating model for #");  Serial.println(id);
        
        p = finger.createModel();
        if (p == FINGERPRINT_OK) {
            Serial.println("Prints matched!");
        } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
            Serial.println("Communication error");
        } else if (p == FINGERPRINT_ENROLLMISMATCH) {
            Serial.println("Fingerprints did not match");
        } else {
            Serial.println("Unknown error");
        }   
        Serial.print("ID "); Serial.println(id);
        p = finger.storeModel(id);
        if (p == FINGERPRINT_OK) {
            Serial.println("Stored!");
            mode=2;
        } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
            Serial.println("Communication error");
        } else if (p == FINGERPRINT_BADLOCATION) {
            Serial.println("Could not store in that location");
        } else if (p == FINGERPRINT_FLASHERR) {
            Serial.println("Error writing to flash");
        } else {
            Serial.println("Unknown error");
        }        
    }
    else if (mode==2){
        p = finger.image2Tz();
        switch (p) {
            case FINGERPRINT_OK:
            Serial.println("Image converted");
            break;
            case FINGERPRINT_IMAGEMESS:
            Serial.println("Image too messy");
            break;
            case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            break;
            case FINGERPRINT_FEATUREFAIL:
            Serial.println("Could not find fingerprint features");
            break;
            case FINGERPRINT_INVALIDIMAGE:
            Serial.println("Could not find fingerprint features");
            break;
            default:
            Serial.println("Unknown error");
            break;
        }
        p = finger.fingerSearch();
        if (p == FINGERPRINT_OK) {
            Serial.println("Found a print match!");
        } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
            Serial.println("Communication error");
        } else if (p == FINGERPRINT_NOTFOUND) {
            Serial.println("Did not find a match");
        } else {
            Serial.println("Unknown error");
        }
        Serial.print("Found ID #"); Serial.print(finger.fingerID);
        Serial.print(" with confidence of "); Serial.println(finger.confidence);
    }
}
