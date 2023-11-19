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
    while (1); // Sonsuz döngüde kal
  }
  nfc.SAMConfig();
  Serial.println("Waiting for an NFC card ...");
}

void loop(void) {
  static bool cardProcessed = false; // Kart işleme durumu

  if (cardProcessed) {
    return; // Eğer kart zaten işlendi ise loop fonksiyonundan çık
  }

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("Found an NFC card!");

    // Veri (boyutunu ve içeriğini projenize uygun olarak ayarlayın)
    uint8_t longData[1220] = {0x7b, 0x22, 0x61, 0x74, 0x74, 0x65, 0x73, 0x74, 0x65, 0x72, 0x22, 0x3a, 0x22, 0x53, 0x6f, 0x63, 0x69, 0x61, 
    0x6c, 0x4b, 0x59, 0x43, 0x22, 0x2c, 0x22, 0x63, 0x54, 0x79, 0x70, 0x65, 0x54, 0x69, 0x74, 0x6c, 0x65, 0x22, 0x3a, 0x22, 0x45, 0x6d, 0x61, 
    0x69, 0x6c, 0x22, 0x2c, 0x22, 0x63, 0x72, 0x65, 0x64, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x22, 0x3a, 0x7b, 0x22, 0x63, 0x6c, 0x61, 0x69, 
    0x6d, 0x22, 0x3a, 0x7b, 0x22, 0x63, 0x54, 0x79, 0x70, 0x65, 0x48, 0x61, 0x73, 0x68, 0x22, 0x3a, 0x22, 0x30, 0x78, 0x33, 0x32, 0x39, 0x31, 
    0x62, 0x62, 0x31, 0x32, 0x36, 0x65, 0x33, 0x33, 0x62, 0x34, 0x38, 0x36, 0x32, 0x64, 0x34, 0x32, 0x31, 0x62, 0x66, 0x61, 0x61, 0x31, 0x64, 
    0x32, 0x66, 0x32, 0x37, 0x32, 0x65, 0x36, 0x63, 0x64, 0x66, 0x63, 0x34, 0x66, 0x39, 0x36, 0x36, 0x35, 0x38, 0x39, 0x38, 0x38, 0x66, 0x62, 
    0x63, 0x66, 0x66, 0x65, 0x61, 0x38, 0x39, 0x31, 0x34, 0x62, 0x64, 0x39, 0x61, 0x63, 0x22, 0x2c, 0x22, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 
    0x74, 0x73, 0x22, 0x3a, 0x7b, 0x22, 0x45, 0x6d, 0x61, 0x69, 0x6c, 0x22, 0x3a, 0x22, 0x61, 0x79, 0x62, 0x61, 0x72, 0x73, 0x61, 0x79, 0x61, 
    0x6e, 0x40, 0x67, 0x6d, 0x61, 0x69, 0x6c, 0x2e, 0x63, 0x6f, 0x6d, 0x22, 0x7d, 0x2c, 0x22, 0x6f, 0x77, 0x6e, 0x65, 0x72, 0x22, 0x3a, 0x22, 
    0x64, 0x69, 0x64, 0x3a, 0x6b, 0x69, 0x6c, 0x74, 0x3a, 0x34, 0x72, 0x39, 0x41, 0x53, 0x4e, 0x71, 0x69, 0x55, 0x4e, 0x71, 0x36, 0x52, 0x78, 
    0x76, 0x58, 0x67, 0x66, 0x6d, 0x6b, 0x56, 0x38, 0x33, 0x66, 0x67, 0x76, 0x32, 0x57, 0x38, 0x6e, 0x57, 0x75, 0x43, 0x38, 0x77, 0x74, 0x77, 
    0x6f, 0x6d, 0x70, 0x53, 0x66, 0x51, 0x35, 0x65, 0x5a, 0x63, 0x6b, 0x22, 0x7d, 0x2c, 0x22, 0x63, 0x6c, 0x61, 0x69, 0x6d, 0x48, 0x61, 0x73, 
    0x68, 0x65, 0x73, 0x22, 0x3a, 0x5b, 0x22, 0x30, 0x78, 0x36, 0x66, 0x32, 0x31, 0x63, 0x38, 0x30, 0x38, 0x30, 0x37, 0x30, 0x33, 0x32, 0x65, 
    0x30, 0x61, 0x32, 0x66, 0x66, 0x39, 0x35, 0x37, 0x38, 0x62, 0x34, 0x64, 0x34, 0x35, 0x32, 0x64, 0x32, 0x63, 0x33, 0x35, 0x33, 0x63, 0x34, 
    0x65, 0x37, 0x61, 0x34, 0x39, 0x31, 0x64, 0x35, 0x36, 0x36, 0x30, 0x62, 0x36, 0x65, 0x30, 0x66, 0x61, 0x39, 0x66, 0x34, 0x36, 0x65, 0x35, 
    0x35, 0x36, 0x31, 0x30, 0x22, 0x2c, 0x22, 0x30, 0x78, 0x66, 0x63, 0x37, 0x37, 0x65, 0x66, 0x36, 0x62, 0x32, 0x33, 0x37, 0x30, 0x62, 0x63, 
    0x61, 0x32, 0x63, 0x35, 0x30, 0x36, 0x61, 0x33, 0x63, 0x32, 0x36, 0x63, 0x37, 0x62, 0x64, 0x35, 0x34, 0x63, 0x61, 0x33, 0x34, 0x38, 0x37, 
    0x61, 0x39, 0x37, 0x39, 0x34, 0x31, 0x66, 0x66, 0x30, 0x62, 0x65, 0x36, 0x35, 0x61, 0x35, 0x33, 0x63, 0x35, 0x37, 0x62, 0x62, 0x37, 0x37, 
    0x32, 0x31, 0x39, 0x62, 0x22, 0x5d, 0x2c, 0x22, 0x63, 0x6c, 0x61, 0x69, 0x6d, 0x4e, 0x6f, 0x6e, 0x63, 0x65, 0x4d, 0x61, 0x70, 0x22, 0x3a, 
    0x7b, 0x22, 0x30, 0x78, 0x30, 0x30, 0x66, 0x61, 0x35, 0x36, 0x30, 0x35, 0x39, 0x61, 0x31, 0x30, 0x33, 0x61, 0x32, 0x65, 0x35, 0x39, 0x65, 
    0x35, 0x38, 0x65, 0x37, 0x32, 0x62, 0x61, 0x32, 0x38, 0x64, 0x33, 0x34, 0x39, 0x37, 0x38, 0x65, 0x37, 0x30, 0x31, 0x34, 0x62, 0x30, 0x36, 
    0x64, 0x37, 0x30, 0x38, 0x36, 0x36, 0x30, 0x64, 0x63, 0x66, 0x66, 0x30, 0x31, 0x30, 0x36, 0x62, 0x30, 0x35, 0x30, 0x65, 0x30, 0x39, 0x22, 
    0x3a, 0x22, 0x37, 0x37, 0x30, 0x35, 0x64, 0x38, 0x61, 0x61, 0x2d, 0x64, 0x63, 0x66, 0x64, 0x2d, 0x34, 0x39, 0x31, 0x66, 0x2d, 0x62, 0x38, 
    0x31, 0x35, 0x2d, 0x65, 0x64, 0x32, 0x64, 0x61, 0x34, 0x33, 0x37, 0x63, 0x66, 0x66, 0x66, 0x22, 0x2c, 0x22, 0x30, 0x78, 0x39, 0x63, 0x62, 
    0x64, 0x66, 0x66, 0x33, 0x64, 0x38, 0x31, 0x62, 0x62, 0x61, 0x38, 0x34, 0x32, 0x66, 0x32, 0x38, 0x63, 0x30, 0x34, 0x39, 0x38, 0x30, 0x30, 
    0x64, 0x33, 0x36, 0x37, 0x33, 0x38, 0x32, 0x30, 0x66, 0x36, 0x39, 0x34, 0x30, 0x61, 0x37, 0x30, 0x65, 0x62, 0x37, 0x64, 0x31, 0x66, 0x39, 
    0x38, 0x34, 0x32, 0x62, 0x37, 0x66, 0x66, 0x36, 0x32, 0x61, 0x65, 0x38, 0x30, 0x61, 0x65, 0x22, 0x3a, 0x22, 0x33, 0x32, 0x66, 0x63, 0x62, 
    0x65, 0x62, 0x64, 0x2d, 0x30, 0x64, 0x64, 0x31, 0x2d, 0x34, 0x39, 0x33, 0x32, 0x2d, 0x61, 0x32, 0x31, 0x38, 0x2d, 0x64, 0x38, 0x39, 0x34, 
    0x62, 0x61, 0x38, 0x63, 0x34, 0x36, 0x38, 0x61, 0x22, 0x7d, 0x2c, 0x22, 0x64, 0x65, 0x6c, 0x65, 0x67, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x49, 
    0x64, 0x22, 0x3a, 0x6e, 0x75, 0x6c, 0x6c, 0x2c, 0x22, 0x6c, 0x65, 0x67, 0x69, 0x74, 0x69, 0x6d, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x22, 
    0x3a, 0x5b, 0x5d, 0x2c, 0x22, 0x72, 0x6f, 0x6f, 0x74, 0x48, 0x61, 0x73, 0x68, 0x22, 0x3a, 0x22, 0x30, 0x78, 0x61, 0x33, 0x37, 0x38, 0x37, 
    0x31, 0x36, 0x64, 0x37, 0x37, 0x37, 0x31, 0x30, 0x62, 0x64, 0x66, 0x32, 0x63, 0x34, 0x36, 0x35, 0x33, 0x31, 0x37, 0x39, 0x65, 0x65, 0x31, 
    0x62, 0x64, 0x64, 0x64, 0x30, 0x39, 0x65, 0x39, 0x36, 0x32, 0x63, 0x36, 0x36, 0x31, 0x33, 0x62, 0x66, 0x61, 0x39, 0x61, 0x32, 0x34, 0x35, 
    0x39, 0x61, 0x32, 0x35, 0x61, 0x35, 0x36, 0x37, 0x37, 0x36, 0x36, 0x66, 0x39, 0x22, 0x7d, 0x2c, 0x22, 0x6e, 0x61, 0x6d, 0x65, 0x22, 0x3a, 
    0x22, 0x45, 0x6d, 0x61, 0x69, 0x6c, 0x20, 0x31, 0x22, 0x2c, 0x22, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x22, 0x3a, 0x22, 0x61, 0x74, 0x74, 
    0x65, 0x73, 0x74, 0x65, 0x64, 0x22, 0x7d};

    // Yazılacak blokların başlangıç ve bitiş blok numaraları
    uint8_t startBlock = 144;
    uint8_t endBlock = 255;

    // Veri yazma işlemi - write_slot1 veya write_slot2'yi burada seçin
    write_slot2_for_4k(uid, uidLength, longData, startBlock, endBlock);
    // write_slot2(uid, uidLength, longData, startBlock, endBlock);

    cardProcessed = true; // Kart işlendi olarak işaretle
  }
}

void write_slot1(uint8_t *uid, uint8_t uidLength, uint8_t *longData, uint8_t startBlock, uint8_t endBlock) {
  uint8_t dataIndex = 0;
  for (uint8_t block = startBlock; block <= endBlock; block++) {
    // Her dört blokta bir blok atla
    if ((block - startBlock) % 4 == 3) continue;

    uint8_t dataToWrite[16];
    memcpy(dataToWrite, &longData[dataIndex * 16], 16);
    dataIndex++;

    // Bloğu doğrula ve yaz
    if (!writeBlock(uid, uidLength, block, dataToWrite)) {
      Serial.print("Failed to write data to block "); Serial.println(block);
      // Yazma işlemi başarısız oldu, döngüden çık
      break;
    }
  }
}

void write_slot2_for_4k(uint8_t *uid, uint8_t uidLength, uint8_t *longData, uint8_t startSector, uint8_t endSector) {
  uint8_t dataIndex = 0;
  for (uint8_t sector = startSector; sector <= endSector; sector++) {
    int startBlock = (sector < 32) ? sector * 4 : 128 + (sector - 32) * 16;
    int endBlock = (sector < 32) ? startBlock + 3 : startBlock + 15;

    for (int block = startBlock; block <= endBlock; block++) {
      // Her 16 blokta bir blok atla
      if ((block - startBlock) % 16 == 15) continue;

      uint8_t dataToWrite[16];
      memcpy(dataToWrite, &longData[dataIndex * 16], 16);
      dataIndex++;

      // Bloğu doğrula ve yaz
      if (!writeBlock(uid, uidLength, block, dataToWrite)) {
        Serial.print("Failed to write data to block "); Serial.println(block);
        // Yazma işlemi başarısız oldu, döngüden çık
        return;
      }
    }
  }
}




bool writeBlock(uint8_t *uid, uint8_t uidLength, uint8_t blockNumber, uint8_t *data) {
  uint8_t key_a[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  bool success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, blockNumber, 0, key_a);
  if (!success) {
    Serial.print("Failed to authenticate block "); Serial.println(blockNumber);
    return false;
  }

  success = nfc.mifareclassic_WriteDataBlock(blockNumber, data);
  if (!success) {
    Serial.print("Failed to write data to block "); Serial.println(blockNumber);
    return false;
  }

  Serial.print("Written to block "); Serial.println(blockNumber);
  return true;
}

void readBlocks(uint8_t *uid, uint8_t uidLength, uint8_t startBlock, uint8_t endBlock) {
  uint8_t key_a[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  Serial.println("Reading sectors...");
  for (uint8_t block = startBlock; block <= endBlock; block++) {
    if (!nfc.mifareclassic_AuthenticateBlock(uid, uidLength, block, 0, key_a)) {
      Serial.print("Failed to authenticate block "); Serial.println(block);
      continue; // Bu bloğu atla ve sonrakine geç
    }

    uint8_t data[16];
    if (!nfc.mifareclassic_ReadDataBlock(block, data)) {
      Serial.print("Failed to read block "); Serial.println(block);
      continue; // Bu bloğu atla ve sonrakine geç
    }

    Serial.print("Block "); Serial.print(block); Serial.print(": ");
    for (uint8_t i = 0; i < 16; i++) {
      Serial.print(data[i], HEX); Serial.print(" ");
    }
    Serial.println("");
  }
}