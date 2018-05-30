#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D8;     // Configurable, see typical pin layout above
const char* ssid = "University of Washington";
const char* host = "52.158.236.197";

char upload1[50];
char upload2[50];

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];

byte n = 0;

void setup() { 
  Serial.begin(115200);

  delay(10);
  
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, NULL);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  memset(&upload1[0], 0, 50);
  memset(&upload2[0], 0, 50);
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  
}
 
void loop() {
  //char buffer1[10];
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);

  MFRC522::StatusCode result = rfid.PICC_RequestA(bufferATQA, &bufferSize);
  Serial.print("request:");
  Serial.println(result); 

  
  if(result == 0){
  // Verify if the NUID has been readed
    //if(upload1[0] != NULL){
    if(n == 0){
      n = 1;
    }

    else if(n == 1){
      n = 2;
    }
    //}
    if (rfid.PICC_ReadCardSerial()){

      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
        if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
          piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
          piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
          Serial.println(F("Your tag is not of type MIFARE Classic."));
          return;
        }
      
        //Serial.println(F("The NUID tag is:"));
      //printHex(upload2, rfid.uid.size);
      //bin_to_strhex(rfid.uid.uidByte, rfid.uid.size,upload1);
          if(n == 1){
            sprintf(upload1, "%X %X %X %X", rfid.uid.uidByte[0],rfid.uid.uidByte[1],rfid.uid.uidByte[2],rfid.uid.uidByte[3] );
            Serial.print(F("upload1:"));
      //printHex(rfid.uid.uidByte, rfid.uid.size);
            Serial.printf(upload1);
            Serial.println();
          }

          if(n == 2){
            sprintf(upload2, "%X %X %X %X", rfid.uid.uidByte[0],rfid.uid.uidByte[1],rfid.uid.uidByte[2],rfid.uid.uidByte[3] );
            Serial.print(F("upload2:"));
      //printHex(rfid.uid.uidByte, rfid.uid.size);
            Serial.printf(upload2);
            Serial.println();
          }
      
   // else Serial.println(F("Card read previously."));

  // Halt PICC
      rfid.PICC_HaltA();

  // Stop encryption on PCD
      rfid.PCD_StopCrypto1();
    
    }
  }


    else{
        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://10.19.121.9:3000/api/racks"); //HTTP
        JsonArray& UIDlist = JSONencoder.createNestedArray("tagIdList"); //JSON array
        JsonArray& ReaderList = JSONencoder.createNestedArray("readerId"); 
        UIDlist.add(upload1);
        UIDlist.add(upload2);
        ReaderList.add("01");
      //JSONencoder.printTo(Serial);
        Serial.println();
        Serial.print(F("upload1:"));
      //printHex(rfid.uid.uidByte, rfid.uid.size);
        Serial.printf(upload1);
        Serial.println();
        char JSONmessageBuffer[300];
        JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        Serial.println(JSONmessageBuffer);
        http.addHeader("Content-Type", "application/json"); //Specify content-type header
        int httpCode = http.POST(JSONmessageBuffer); //Send the request
        String payload = http.getString(); //Get the response payload
        Serial.println(httpCode); //Print HTTP return code
        Serial.println(payload); //Print request response payload
        http.end();
      //Close connection 
      n = 0; 
      rfid.PCD_AntennaOff();
      rfid.PCD_AntennaOn();
      delay(500);
      memset(&upload1[0], 0, 50);
      memset(&upload2[0], 0, 50);
    }   
}
/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

