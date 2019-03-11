/*
 * ----------------------------------------------------------------------
 * Example program showing how to read new NUID from a PICC to serial.
 * ----------------------------------------------------------------------
 * https://circuits4you.com
 * 
 * RC522 Interfacing with NodeMCU
 * 
 * Typical pin layout used:
 * ----------------------------------
 *             MFRC522      Node     
 *             Reader/PCD   MCU      
 * Signal      Pin          Pin      
 * ----------------------------------
 * RST/Reset   RST          D1 (GPIO5)        
 * SPI SS      SDA(SS)      D2 (GPIO4)       
 * SPI MOSI    MOSI         D7 (GPIO13)
 * SPI MISO    MISO         D6 (GPIO12)
 * SPI SCK     SCK          D5 (GPIO14)
 * 3.3V        3.3V         3.3V
 * GND         GND          GND
 */

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

#include <FirebaseArduino.h>

//#define FIREBASE_HOST "dashkart-b8214.firebaseio.com"
//#define FIREBASE_AUTH "Brb6pjzyhOrKj10IPDeHhYU3GbHeCPNQtub95Grq"
#define WIFI_SSID "gpn"
#define WIFI_PASSWORD "happygpn"

constexpr uint8_t RST_PIN = 5;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 4;     // Configurable, see typical pin layout above
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];

void setup() { 
   Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output


  //////////////////////////////////
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("connecting");
while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}
Serial.println();
Serial.print("connected: ");
Serial.println(WiFi.localIP());
//Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  ///////////////////////////////////
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

//  Serial.println(F("This code scan the MIFARE Classsic NUID."));///////////////////////////////////
//  Serial.print(F("Using the following key:"));////////////////////////
//  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}
 
void loop() {

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

//  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
//  Serial.println(rfid.PICC_GetTypeName(piccType));///////////////////////////////////

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
//    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

//  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
//    rfid.uid.uidByte[1] != nuidPICC[1] || 
//    rfid.uid.uidByte[2] != nuidPICC[2] || 
//    rfid.uid.uidByte[3] != nuidPICC[3] ) {
//    Serial.println(F("A new card has been detected."));
//
//    // Store NUID into nuidPICC array
//    for (byte i = 0; i < 4; i++) {
//      nuidPICC[i] = rfid.uid.uidByte[i];
//    }
   
//    Serial.println(F("The NUID tag is:"));/////////////////////////////
//    Serial.print(F("In hex: "));
//    printHex(rfid.uid.uidByte, rfid.uid.size);
//    Serial.println();
//    Serial.print(F("In dec: "));////////////////////////////////
//    printDec(rfid.uid.uidByte, rfid.uid.size);/////////////////////////////////
    ////////////////////////////////////
    char str[32] = "";
  array_to_string(rfid.uid.uidByte,  rfid.uid.size, str);
//  Serial.println("");///////////////////////////////////////////////////////
  Serial.println(str);
  
//  String name = Firebase.pushString("string",str);
  
////handle error
//if (Firebase.failed()) {
//Serial.print("pushing /logs failed:");
//Serial.println(Firebase.error()); 
//return;
//}
 digitalWrite(LED_BUILTIN, LOW);  
 delay(100);
 digitalWrite(LED_BUILTIN, HIGH); 
 delay(100);                      


//Serial.print("pushed: /logs/");
//Serial.println(name);
delay(1000);
/////////////////////////////////////////////////////
//  }
//  else Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
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
  char final[20];
//  Serial.print(buffer);
int n=20;
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i]);
//      Serial.print(i);

    
    
    
  }
}
void array_to_string(byte array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}
