#include <Wire.h>
#include <Adafruit_PN532.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1);
  }
  nfc.SAMConfig();
  Serial.println("Waiting for an NFC card ...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("Found an NFC card!");

    // Display some basic information about the card
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x");Serial.print(uid[i], HEX);
    }
    Serial.println("");

    uint8_t key_a[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    Serial.println("Sector Block   0  1  2  3   4  5  6  7   8  9 10 11  12 13 14 15  AccessBits");

    for (uint16_t blockNumber = 0; blockNumber < 512; blockNumber++) { // 512 blocks for 8K card
      // Authenticate and read data
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, blockNumber, 0, key_a);

      if (success) {
        uint8_t readdata[16];
        success = nfc.mifareclassic_ReadDataBlock(blockNumber, readdata);

        if (success) {
          // Print sector/block information
          Serial.print("  "); Serial.print(blockNumber / 4);      // Sector
          Serial.print("     "); Serial.print(blockNumber % 4);   // Block
          Serial.print("   ");

          // Print data
          for (uint8_t i = 0; i < 16; i++) {
            if (i == 4 || i == 8 || i == 12) {
              Serial.print("  ");
            }
            Serial.print(" "); Serial.print(readdata[i], HEX); if (readdata[i] < 0x10) Serial.print("0");
          }

          // Print access bits
          Serial.print("  [ ");
          if (blockNumber % 4 != 3) {
            Serial.print("0 0 0"); // Regular data blocks
          } else {
            Serial.print("0 0 1"); // Access bits
          }
          Serial.print(" ] ");
          Serial.println();
        } else {
          Serial.print("Failed to read data from block "); Serial.println(blockNumber);
        }
      } else {
        Serial.print("Failed to authenticate block "); Serial.println(blockNumber);
      }
    }
    delay(100000); // 10 seconds delay to prevent continuous reading
  }
}
