#ifndef krapp_utils
#define krapp_utils

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#include <WiFi.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels
#define doorNumber 2 // numero de puerta que le paso a la API

String getUID(MFRC522& rfid_mfrc522);

void readingData(MFRC522& rfid_mfrc522, MFRC522::MIFARE_Key& key, MFRC522::StatusCode& status, int SIZE_BUFFER, int MAX_SIZE_BLOCK, int redPin, int greenPin);

String getSerialStringInput();

#endif