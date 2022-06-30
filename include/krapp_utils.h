#ifndef krapp_utils
#define krapp_utils

#include <Arduino.h>
#include <ArduinoLog.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#include <WiFi.h>
#include <iostream>
#include <time.h>
#include <vector>

String getDateTime();

String getUID(MFRC522& rfid_mfrc522);

void readingData(MFRC522& rfid_mfrc522, MFRC522::MIFARE_Key& key, MFRC522::StatusCode& status, int SIZE_BUFFER, int MAX_SIZE_BLOCK, int redPin, int greenPin);

String getSerialStringInput();

int menu();

void notFound(AsyncWebServerRequest* request);

String processor(const String& var);

#endif