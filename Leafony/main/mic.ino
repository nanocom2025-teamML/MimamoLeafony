void setupMIC() {
  pinMode(MIC, INPUT);
  systemLog("MIC", "OK", 1, 1);
}
void getMIC() { dataMIC = analogRead(MIC); }

// void saveMIC() {
//   if (dataFile) {
//     dataFile.println(dataMIC);
//   } else {
//     progStop("ファイル書き込み失敗");
//   }
// }

void saveMICbuffered() {
  if (dataFile) {
    for (unsigned int i = 0; i < mic_buf_idx; i++) {
      dataFile.println(mic_buffer[i]);
    }
    dataFile.flush();
  } else {
    progStop("ファイル書き込み失敗");
  }
}

void recordMICbuffered(const int &khz, const int &sec) {
  systemLog("Record","START",2,2);
  unsigned long total_samples = khz * sec * 1000;
  mic_buf_idx = 0;
  for (unsigned long i = 0; i < total_samples; i++) {
    mic_buffer[mic_buf_idx++] = analogRead(MIC);
    if (mic_buf_idx >= MIC_BUFFER_SIZE) {
      sendMessage();  // サーバー送信
      // save_buffered_MIC();  // ファイル保存
      mic_buf_idx = 0;
    }
    delayMicroseconds(1000. / (float)khz);
  }
  // 残りのデータも保存
  if (mic_buf_idx > 0) {
    sendMessage();
    // save_buffered_MIC();
  }
  systemLog("Record","FINISH",2,2);
}

// マイクデータの送信
void sendMessage() {
  String json = "{\"audio_data\":[";
  for (size_t i = 0; i < mic_buf_idx; i++) {
    json += String(mic_buffer[i]);
    if (i < mic_buf_idx - 1) json += ",";
  }
  json += "]}";
  sendJson("/api/device/messages", json);
}