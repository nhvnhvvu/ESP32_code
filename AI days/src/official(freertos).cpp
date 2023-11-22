#include <Adafruit_Fingerprint.h>
#include<PubSubClient.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h> 


#define mySerial Serial2

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
SemaphoreHandle_t xSemaphoreNew = NULL,xSemaphoreDelete = NULL;

const char* pub_topic = "3CSLAB/AIDAYS/receive";
const char* ssid = "AIDAYS";
const char* password = "12345678";

String host = "http://192.168.4.1/AIDAYS/httphandle.php";
const char* mqtt_server = "Broker.hivemq.com";


WiFiClient espClient;
PubSubClient client(espClient);

int p = -1;
int delete_id = 0;

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

  if (String(topic) == "3CSLAB/AIDAYS/new") { 
    if( xSemaphoreGive( xSemaphoreNew ) != pdTRUE )
    {
        Serial.println("SMPNew was not given");
    }
  }
  else if (String(topic) == "3CSLAB/AIDAYS/delete") {
    if(sscanf(messageTemp.c_str(), "{\"delete\":%d}", &delete_id)>0){
      if( xSemaphoreGive( xSemaphoreDelete ) != pdTRUE )
      {
          Serial.println("SMPDel was not given");
      }

    }
  }
  // else if (String(topic) == "3CSLab/AIday/output") {
  //   char name[30];
  //   if(sscanf(messageTemp.c_str(), "{\"output\":%s}", name)>0){
  //     Serial.print("LCD:");
  //     Serial.println(name);  //LCD
  //   }
  // }
}

String httpGETRequest(String serverName){
  HTTPClient http;
  http.begin(serverName);
  int responseCode = http.GET();
  String responseString = "{}";
  if (responseCode > 0) {
      responseString = http.getString();
    }
  http.end();
  return responseString;
}


uint8_t deleteFingerprint(uint8_t id) {
  Serial.print("Deleting ID #");
  Serial.println(id);
  uint8_t p = -1;
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
  }

  return p;
}

void connect_mqtt(){
    if (!client.connected()) {
        while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("Duong's ESP")) {
            Serial.println("connected");
            client.subscribe("3CSLAB/AIDAYS/new");
            client.subscribe("3CSLAB/AIDAYS/delete");
            client.subscribe("3CSLAB/AIDAYS/output");
            Serial.println("Sub to 3cslab");
            }
        else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            vTaskDelay(5000);
            }
        }
    }
}

void add_new_fingerprint(){
    Serial.println("Add new finger");
    p = finger.getImage();
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
        default:
        Serial.println("ERROR 1");
        break;
        }
    }
    p = finger.image2Tz(1);
    switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;

        default:
        Serial.println("ERROR 2");
        break;
    }
    Serial.println("Remove finger");
    finger.getTemplateCount();
    delay(1000);
    int id = finger.templateCount+1;
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

        default:
        Serial.println("ERROR 3");
        break;
        }
    }
    p = finger.image2Tz(2);
    switch (p) {
        case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
        default:
        Serial.println("ERROR 4");
        break;
    }
    Serial.print("Creating model for #");  Serial.println(id);
    
    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
        Serial.println("Prints matched!");
    }  else {
        Serial.println("ERROR 5");
    }   
    Serial.print("ID "); Serial.println(id);
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
        Serial.println("Stored!");
        String payload = "{\"UID\":"+String(id)+"}";
        Serial.println(payload);
        client.publish(pub_topic, payload.c_str());

    } else {
        Serial.println("ERROR 6");
    }
}

void setup()  
{
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(500);
        Serial.print(".");
    }
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);  
    // pinMode(2,OUTPUT);
    // digitalWrite(2,HIGH);
    finger.begin(57600);
    if (finger.verifyPassword()) {
        Serial.println("Found fingerprint sensor!");
    } else {
        Serial.println("Did not find fingerprint sensor :(");
        while (1) { delay(1); }
    }
    connect_mqtt();
    xSemaphoreNew = xSemaphoreCreateBinary();
    xSemaphoreDelete = xSemaphoreCreateBinary();
}



uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

 

void loop()
{
    getFingerprintID();
    connect_mqtt();
    client.loop();
    // vTaskDelay(50);
    if( xSemaphoreTake( xSemaphoreNew, ( TickType_t ) 10 ) == pdTRUE )
        {   
          add_new_fingerprint();
        }
    if( xSemaphoreTake( xSemaphoreDelete, ( TickType_t ) 10 ) == pdTRUE )
        {   
          deleteFingerprint(delete_id);
        }

}
