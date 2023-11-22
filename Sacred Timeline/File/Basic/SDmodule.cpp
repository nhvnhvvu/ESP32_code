#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SCK  5
#define MISO  18
#define MOSI  19
#define CS  17

SPIClass spi = SPIClass(VSPI);
int info = 0,data = 0;
unsigned long dtime = 0;

void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

// appendFile(SD, "/data.txt", dataMessage.c_str())
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}


void initSDCard(){
  spi.begin(SCK, MISO, MOSI, CS);
  if (!SD.begin(CS,spi,80000000)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  Serial.printf("SD Card Size: %lluMB\n", SD.cardSize() / (1024 * 1024));
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void setup(){
  Serial.begin(9600);
  initSDCard();
  dtime=millis();
}

void loop(){
  if(millis()-dtime >5000){
    const char * fileDir = ("/" + String(info) + ".txt").c_str();
    createDir(SD,fileDir);
    while(millis()-dtime >5000){
      appendFile(SD,fileDir,)
    }
  }
}


