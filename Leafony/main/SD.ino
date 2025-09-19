// VSPI
// == RTC & microSD Leaf==
#define SD_EN 14  // D6  IO14
#define SS 5      // D10 IO5
#define MOSI 23   // D11 IO23
#define MISO 19   // D12 IO19
#define SCK 18    // D13 IO18
// SDカードの初期化
void setupSD() {
  if (!SD.begin()) {
    progStop("SD card Mount Failed");  // 初期化失敗時は停止
  }
  systemLog("SD CARD", "OK", 1, 1);
  cardInfo();
}

// SDカード情報表示
void cardInfo() {
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    progStop("No SD card Attached");
  }
  const char *type;
  if (cardType == CARD_MMC) {
    type = "MMC";
  } else if (cardType == CARD_SD) {
    type = "SDSC";
  } else if (cardType == CARD_SDHC) {
    type = "SDHC";
  } else {
    type = "UNKNOWN";
  }
  systemLog("SD Card Type", type, 1, 2);

  Serial.printf("-- SD: %lluMB ", SD.cardSize() / (1024 * 1024));
  Serial.printf("Total: %lluMB ", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

// ファイルを開く（なければ新規作成、追記モード）
void fileOpen(const String &filename) {
  dataFile = SD.open(filename, FILE_WRITE);
  if (!dataFile) {
    progStop("File Open Failed");
  }
  systemLog("OPEN", filename);
}

void fileClose() {
  if (dataFile) {
    dataFile.close();
    systemLog("FILE","CLOSE");
  } else {
    systemLog("FILE","NOT OPENED");
  }
}
