// 改行
void logln() {
  if (!testMode) return;
  Serial.println();
}
void logln(int &type, const int &len) {
  if (testMode>len||!testMode) return;
  String head;
  for (int i = 0; i < len; i++) {
    head += symbol[type];
  }
  Serial.println(head);
}
// テンプレ出力
void systemLog(const char *log) { systemLog(log, 2, 2); }
void systemLog(const char *log, const int &type, const int &len) {
  if (testMode>len||!testMode) return;
  String head;
  for (int i = 0; i < len; i++) {
    head += symbol[type];
  }
  head += ' ';
  Serial.print(head);
  Serial.println(log);
}

void systemLog(const char *title, const int &info) {
  systemLog(title, String(info), 2, 2);
}

void systemLog(const char *title, const String &info) {
  systemLog(title, info, 2, 2);
}
void systemLog(const char *title, const int &info, const int &type,
               const int &len) {
  systemLog(title, String(info), type, len);
}
void systemLog(const char *title, const char &info, const int &type,
               const int &len) {
  systemLog(title, String(info), type, len);
}

void systemLog(const char *title, const String &info, const int &type,
               const int &len) {
  if (testMode>len||!testMode) return;
  String head;
  for (int i = 0; i < len; i++) {
    head += symbol[type];
  }
  head += ' ';
  Serial.print(head);
  Serial.print(title);
  Serial.print(" : ");
  Serial.println(info);
}