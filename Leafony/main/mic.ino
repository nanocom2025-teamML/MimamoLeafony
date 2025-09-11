

void setupMIC() { pinMode(MIC, INPUT); }

void get_dataMIC() { dataMIC = analogRead(MIC); }

// void save_dataMIC() {
//   if (dataFile) {
//     dataFile.println(dataMIC);
//   } else {
//     Serial.println("ファイル書き込み失敗");
//     progStop();
//   }
// }

void save_buffered_MIC() {
  if (dataFile) {
    for (unsigned int i = 0; i < mic_buf_idx; i++) {
      dataFile.println(mic_buffer[i]);
    }
    dataFile.flush();
    mic_buf_idx = 0;
  } else {
    Serial.println("ファイル書き込み失敗");
    progStop();
  }
}

void record_MIC(int khz, int sec) {
  Serial.println("録音開始");
  setupMIC();
  unsigned long total_samples = khz * sec * 1000;
  mic_buf_idx = 0;
  for (unsigned long i = 0; i < total_samples; i++) {
    mic_buffer[mic_buf_idx++] = analogRead(MIC);
    if (mic_buf_idx >= MIC_BUFFER_SIZE) {
      sendMessage(mic_buffer, mic_buf_idx);  // サーバー送信
      setupMIC();
      // save_buffered_MIC();  // ファイル保存
    }
    delayMicroseconds(1000. / (float)khz);
  }
  // 残りのデータも保存
  if (mic_buf_idx > 0) {
    sendMessage(mic_buffer, mic_buf_idx);
  }
  Serial.println("録音終了");
  Serial.println("録音データをmicdata.csvに保存しました");
}