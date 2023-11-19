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

    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x");Serial.print(uid[i], HEX);
    }
    Serial.println("");

    uint8_t key_a[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    Serial.println("Sector Block   0  1  2  3   4  5  6  7   8  9 10 11  12 13 14 15  AccessBits");

    // İlk 32 sektörde her sektörde 4 blok, sonrasında her sektörde 16 blok vardır
    for (uint16_t sector = 0; sector < 40; sector++) {
      uint8_t blocksInSector = (sector < 32) ? 4 : 16;

      for (uint8_t blockOffset = 0; blockOffset < blocksInSector; blockOffset++) {
        uint16_t blockNumber = sector * 4 + blockOffset;
        if (sector >= 32) {
          blockNumber = 128 + (sector - 32) * 16 + blockOffset;
        }

        // Authenticate and read data
        success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, blockNumber, 0, key_a);

        if (success) {
          uint8_t readdata[16];
          success = nfc.mifareclassic_ReadDataBlock(blockNumber, readdata);

          if (success) {
            // Print sector/block information
            Serial.print("  "); Serial.print(sector);
            Serial.print("     "); Serial.print(blockOffset);
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
            if (blockOffset != (blocksInSector - 1)) {
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
    }
    delay(10000); // 10 seconds delay to prevent continuous reading
  }
}
