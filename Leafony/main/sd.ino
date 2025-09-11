// VSPI
// SDカードの初期化
void setupSD() {
  if (!SD.begin()) {
    Serial.println("SD card Mount Failed");
    progStop();  // 初期化失敗時は停止
  }
  Serial.println("SD card initialized.");
  cardInfo();
}

// SDカード情報表示
void cardInfo() {
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached.");
    progStop();
  }
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

// ファイルを開く（なければ新規作成、追記モード）
void fileOpen(String filename) {
  dataFile = SD.open(filename, FILE_WRITE);
  if (!dataFile) {
    Serial.println("ファイルオープン失敗");
    progStop();
  }
}

void fileClose() {
  if (dataFile) {
    dataFile.close();
  }
}
