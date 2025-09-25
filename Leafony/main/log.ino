// 改行
void logln() {
  if (!testMode) return;
  Serial.println();
}
void logln(int type, int len) {
  if (testMode > len || !testMode) return;
  String head;
  for (int i = 0; i < len; i++) {
    head += symbol[type];
  }
  Serial.println(head);
}
// テンプレ出力
void systemLog(const char *log) { systemLog(log, 2, 2); }
void systemLog(const char *log, int type, int len) {
  if (testMode > len || !testMode) return;
  String head;
  for (int i = 0; i < len; i++) {
    head += symbol[type];
  }
  head += ' ';
  Serial.print(head);
  Serial.println(log);
}

template <typename T>
void systemLog(const char *title, const T &info) {
  systemLog(title, info, 2, 2);
}

template <typename T>
void systemLog(const char *title, const T &info, int type, int len) {
  if (testMode > len || !testMode) return;

  // ヘッダ作成
  String head;
  for (int i = 0; i < len; i++) {
    head += symbol[type];
  }
  head += ' ';

  // 出力
  Serial.print(head);
  Serial.print(title);
  Serial.print(" : ");
  Serial.println(info);
}
