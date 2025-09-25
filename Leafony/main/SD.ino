// VSPI
// == RTC & microSD==
#define SD_EN 14  // D6  IO14
#define SS 5      // D10 IO5
#define MOSI 23   // D11 IO23
#define MISO 19   // D12 IO19
#define SCK 18    // D13 IO18
// SDカードの初期化
bool setupSD() {
  if (!useSD) {
    return false;
  }
  if (!SD.begin()) {
    systemLog("SD card Mount Failed", 3, 3);  // 初期化失敗時は停止
    useSD = false;
    return false;
  }
  systemLog("SD CARD", "OK", 1, 1);
  cardInfo();
  return true;
}

// SDカード情報表示
bool cardInfo() {
  if (!useSD) {
    return false;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    systemLog("No SD card Attached", 3, 3);
    useSD = false;
    return false;
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
  systemLog("Card Type", type, 1, 2);
  systemLog("SD   (MB)", (int)(SD.cardSize() / (1024 * 1024)), 1, 2);
  systemLog("Total(MB)", (int)(SD.totalBytes() / (1024 * 1024)), 1, 2);
  systemLog("Used (MB)", (int)(SD.usedBytes() / (1024 * 1024)), 1, 2);
  return true;
}

// ファイルを開く（なければ新規作成、追記モード）
bool fileOpen(const String &filename, const char *mode) {
  if (!useSD) {
    systemLog("SD", "Can't Open File", 3, 3);
    return false;
  }
  dataFile = SD.open("/" + filename, mode);
  if (!dataFile) {
    systemLog("SD", "File Open Failed", 3, 3);
    return false;
  }
  systemLog("OPEN", filename, 1, 2);
  return true;
}

bool fileClose() {
  if (!useSD) {
    return false;
  }
  if (dataFile) {
    dataFile.close();
    systemLog("FILE", "CLOSE", 1, 1);
  } else {
    systemLog("FILE", "NOT OPENED", 2, 1);
  }
  return true;
}
