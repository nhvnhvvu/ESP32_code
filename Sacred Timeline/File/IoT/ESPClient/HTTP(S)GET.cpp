#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

 
const char* ssid = "BC68 T2";
const char* password =  "0904465635";
 
// const char* apiURL = "https://project1-bd1a9-default-rtdb.firebaseio.com/Anime.json";
const char* apiURL = "https://firestore.googleapis.com/v1/projects/project1-bd1a9/databases/(default)/documents/1/";

// neu apiURL la https:// thi thu vien se tu dong su dung giao thuc https
// quy trinh cua https: xac thuc SSL/TLS, du lieu dc encrypted
// thu vien se tu dong su dung chung chi SSL/TLS co san trong ROM cua ESP32


// const char* test_root_ca= \
//   "-----BEGIN CERTIFICATE-----\n" \
//   "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n" \
//   "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
//   "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n" \
//   "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n" \
//   "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n" \
//   "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n" \
//   "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n" \
//   "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n" \
//   "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n" \
//   "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n" \
//   "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n" \
//   "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n" \
//   "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n" \
//   "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n" \
//   "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n" \
//   "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n" \
//   "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n" \
//   "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n" \
//   "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n" \
//   "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n" \
//   "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n" \
//   "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n" \
//   "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n" \
//   "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n" \
//   "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n" \
//   "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n" \
//   "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n" \
//   "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n" \
//   "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n" \
//   "-----END CERTIFICATE-----\n";

String httpGETRequest(const char*);
String httpsGETRequest(const char*,const char*);



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
    String responseString= httpGETRequest(apiURL);                   
    JSONVar JSONvar=JSON.parse(responseString);
    Serial.println(JSONvar);
    Serial.println(JSONvar["documents"][0]["fields"]);
    // Serial.println(JSON.typeof_(JSONvar));
    // Serial.println(JSONvar["id"]); // truy cap gia tri trong JSON
    // auto array=JSONvar.keys(); //lay keys duoi dang 1 array
    delay(5000);
    
}



String httpsGETRequest(const char* serverName, const char* test_root_ca){
  WiFiClientSecure *client = new WiFiClientSecure;
  client->setCACert(test_root_ca);  // certificate authority(3 types: root, intermediate, server) 
  //client->setInsecure(); //If you want to skip the SSL server certificate verification, but you still want to have encrypted communication
  HTTPClient http;
  http.begin(*client,serverName);
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

