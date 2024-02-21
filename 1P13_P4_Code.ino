#include <SD.h>
#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>
#include <SPI.h>
#include <MFRC522.h>


// PIN Numbers : RESET + SDAs
#define RST_PIN         9
#define SS_1_PIN        10
#define SS_2_PIN        8

//CS pin on sd card reader
#define SD_ChipSelectPin 4

TMRpcm tmrpcm;


#define NR_OF_READERS   2


byte ssPins[] = {SS_1_PIN, SS_2_PIN};

// Create an MFRC522 instance :
MFRC522 mfrc522[NR_OF_READERS];

byte pepper[8]= {0xb2, 0x04, 0x0e, 0x1b};
byte salt[8]= {0x05, 0x7b, 0xa4, 0xac};

byte cayenne[14] = {0x04, 0xbf, 0xc4, 0xb8, 0x70, 0x00, 0x00};
byte fennel[14] = {0x04, 0x76, 0xc0, 0xb8, 0x70, 0x00, 0x00};
byte garlic_powder[14] = {0x04, 0x95, 0xc0, 0xb8, 0x70, 0x00, 0x00};
byte nutmeg[14] = {0x04, 0xb2, 0xc0, 0xb8, 0x70, 0x00, 0x00};

void setup() {

  Serial.begin(9600);           // Initialize serial communications with the PC
  
  while (!Serial);              // Do nothing if no serial port is opened 

  tmrpcm.speakerPin=9;
  Serial.begin(9600);
  if(!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }
  tmrpcm.setVolume(1);
 

  SPI.begin();                  // Init SPI bus
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
    mfrc522[reader].PCD_DumpVersionToSerial();
  
  }
  Serial.print("Please bring container next to the scanner");
}

void loop() 
{
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    
    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      
      Serial.println();
      Serial.print(F("Card UID:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();

      Serial.println();
      Serial.print("The container is: ");
      if (memcmp(pepper, mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size) == 0)
      {
        Serial.print("pepper");
      }
      else if (memcmp(salt, mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size) == 0)
      {
        Serial.print("salt");
        tmrpcm.play("WANA.WAV");
        delay(1000);
        
      }
      else if (memcmp(cayenne, mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size) == 0)
      {
        Serial.print("Cayenne");
        tmrpcm.play("CAYN.WAV");
        delay(1000);

      }
      else if (memcmp(fennel, mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size) == 0)
      {
        Serial.print("Fennel");
        tmrpcm.play("FNUL.WAV");
        delay(1000);

      }
      else if (memcmp(nutmeg, mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size) == 0)
      {
        Serial.print("Nutmeg");
        tmrpcm.play("NTMG.WAV");
        delay(1000);

      }
      else if (memcmp(garlic_powder, mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size) == 0)
      {
        Serial.print("Garlic Powder");
        tmrpcm.play("GLPD.WAV");
        delay(1000);

      }
      else
      {
        Serial.print("Unknown");
      }

    }
  }
}

/**

dump a byte array as hex values to Serial.
*/
void dump_byte_array(byte *buffer, byte bufferSize) {
for (byte i = 0; i < bufferSize; i++) {
Serial.print(buffer[i] < 0x10 ? " 0" : " ");
Serial.print(buffer[i], HEX);
}
}



