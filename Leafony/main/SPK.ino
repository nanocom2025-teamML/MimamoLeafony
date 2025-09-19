void setupSpeaker() {
  AquesTalk.begin(AT_baudrate, SERIAL_8N1, AT_RX, AT_TX);
  systemLog("SPK&ATP", "OK", 1, 1);
  delay(80);  // 80ms空ける
}
//発声
void speak(const String &text) {
  systemLog("Speak", text,2,2);
  AquesTalk.print(text);  // 発声
  AquesTalk.write(0x0D);  // 改行
  // 発声終了まで待つ
  if (atCheck() == '>') {
    systemLog("Speak End",2,1);
  }
  else{
    systemLog("Speak Error",2,2);
  }
}
// 数字の桁読み
void speak_num(const int &num) {
  String text = "<NUMK VAL=";
  text += num;
  text += ">";
  speak(text);
}
//チャイム
void chime() {
  chime(false);
  chime(true);
}
void chime(const bool &s) { speak(s ? "#J" : "#K"); }
//チェック
char atCheck() {
  char c;
  while (true) {
    if (AquesTalk.available()) {
      c = AquesTalk.read();
      systemLog("ATP", c, 1, 1);
      return c;
    }
  }
}