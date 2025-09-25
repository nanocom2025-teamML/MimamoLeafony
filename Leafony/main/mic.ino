void setupMIC() {
  pinMode(MIC, INPUT);
  
  systemLog("MIC", "OK", 1, 1);
  mic_buffer[0] = analogRead(MIC);
}

void recordMICbuffered(int khz, int sec) {
  fileOpen(AUDIO_FILE_NAME, FILE_WRITE);
  chime(true);
  systemLog("Record", "START", 2, 2);

  const unsigned long total_samples = khz * sec * 1000UL;
  mic_buf_idx = 0;
  const unsigned long interval_us =
      1000UL / khz;                           // サンプリング間隔（マイクロ秒）
  unsigned long next_sample_time = micros();  // 現在の時間を設定
  // delayMicroseconds
  for (unsigned long i = 0; i < total_samples; i++) {  // 総サンプル数を繰り返す
    mic_buffer[mic_buf_idx++] = analogRead(MIC);       // サンプル取得
    if (mic_buf_idx >= MIC_BUFFER_SIZE) {              // バッファ満杯なら保存
      saveMICbuffered();                               // ファイル保存
      mic_buf_idx = 0;                                 // バッファをリセット
    }
    next_sample_time += interval_us;  // 次のサンプル時刻を設定
    long remain = (long)(next_sample_time - micros());  // 残り時間を計算
    if (remain > 0) {
      delayMicroseconds((unsigned long)remain);
    }
  }
  // delayを利用しない方法（他処理可能）
  // for (unsigned long i = 0; i < total_samples;) {  // 総サンプル数を繰り返す
  //   unsigned long now = micros();                  // 現在の時間を取得
  //   if ((long)(now - next_sample_time) >= 0) {     //
  //   サンプリング間隔を経過したら
  //     mic_buffer[mic_buf_idx++] = analogRead(MIC); // サンプル取得
  //     i++;                                         // サンプル数追加
  //     next_sample_time += interval_us;             // サンプリング間隔を追加
  //     if (mic_buf_idx >= MIC_BUFFER_SIZE) {        // バッファ満杯なら保存
  //       saveMICbuffered();                         // ファイル保存
  //       mic_buf_idx = 0;                           // バッファをリセット
  //     }
  //   }
  // }
  if (mic_buf_idx > 0) {  // 残りのデータを保存
    saveMICbuffered();
    mic_buf_idx = 0;
  }
  fileClose();
  chime(false);
  systemLog("Record", "FINISH", 2, 2);
}

// SDカードにマイクバッファをバイナリデータとして保存
void saveMICbuffered() {
  if (dataFile, FILE_WRITE) {
    dataFile.write(
        (uint8_t *)mic_buffer,
        mic_buf_idx *
            sizeof(uint16_t));  // mic_buffer をバイナリとしてまとめて書き込む
    dataFile.flush();           // flushは1回だけでOK
  } else {
    progStop("File Write Failed");
  }
}
