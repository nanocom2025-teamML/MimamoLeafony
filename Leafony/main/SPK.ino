// == ATP3012 & SPEAKER ==
#define AT_BAUDRATE 9600  // 既定値9600bps
#define AT_RX 16          // D9 IO16 - ESP32 RX（ATP3012 TXD）
#define AT_TX 17          // D8 IO17 - ESP32 TX（ATP3012 RXD）
#define AT_SLEEP 15
#define AT_RESPONSE_SIZE 5

HardwareSerial AquesTalk(2);  // AT3012 UART2接続
char atResponse[AT_RESPONSE_SIZE + 1];

void setupSpeaker() {
  pinMode(AT_SLEEP,OUTPUT);
  digitalWrite(AT_SLEEP,HIGH);
  AquesTalk.begin(AT_BAUDRATE, SERIAL_8N1, AT_RX, AT_TX);
  systemLog("SPK&ATP", "OK", 1, 1);
  delay(80);
}
void turnOffSpeaker(){
  digitalWrite(AT_SLEEP,LOW);
}
// 発声
void speak(const String& text) {
  systemLog("Speak", text, 2, 1);
  AquesTalk.print(text);  // 発声
  AquesTalk.write(0x0D);  // 改行
  // 発声終了まで待つ
  if (atCheck()) {
    if (atErrorCheck()) {
      systemLog("Speak End", 2, 1);
    } else {
      systemLog("Speak", "Error", 3, 3);
    }
  } else {
    systemLog("Speak", "TIME OUT", 3, 3);
  }
}

// 数字の桁読み
void speak_num(int num) {
  String text = "<NUMK VAL=";
  text += num;
  text += ">";
  speak(text);
}
// チャイム
void chime() {
  chime(false);
  chime(true);
}
void chime(bool s) { speak(s ? "#J" : "#K"); }

// ATPからのレスポンスをチェック
// 　ATPから>を取得:true タイムアウト:false
bool atCheck() { return atCheck(15 * 1000); }
bool atCheck(unsigned long timeout_ms) {
  unsigned long end_time = millis() + timeout_ms;
  int idx = 0;
  while (true) {
    // タイムアウト判定
    if ((long)(millis() - end_time) >= 0) {
      return false;
    }
    while (AquesTalk.available()) {
      char c = AquesTalk.read();
      // '*' は無視、バッファ上限チェック
      if (c == '*') {
        systemLog("ATP", c, 1, 1);
      } else if (idx < AT_RESPONSE_SIZE) {
        atResponse[idx++] = c;
      }
      if (c == '>') {  // 応答終了判定
        atResponse[idx] = '\0';
        systemLog("ATP", atResponse, 1, 1);
        return true;
      }
    }
  }
}
// エラー判定
// 正常終了:true エラー:false
bool atErrorCheck() {
  // エラーコード判定
  if (atResponse[0] == 'E') {
    const char* msg = "Unknown error";

    if (strncmp(atResponse, "E100", 4) == 0) {
      msg = "Other error";
    } else if (strncmp(atResponse, "E101", 4) == 0) {
      msg = "Out of memory";
    } else if (strncmp(atResponse, "E105", 4) == 0) {
      msg = "Undefined phoneme in text";
    } else if (strncmp(atResponse, "E106", 4) == 0) {
      msg = "Invalid tag in text";
    } else if (strncmp(atResponse, "E107", 4) == 0) {
      msg = "Tag length exceeded or '>' not found";
    } else if (strncmp(atResponse, "E200", 4) == 0) {
      msg = "Text too long";
    } else if (strncmp(atResponse, "E251", 4) == 0) {
      msg = "Invalid command";
    } else if (strncmp(atResponse, "E254", 4) == 0) {
      msg = "Chime data corrupted";
    } else if (strncmp(atResponse, "E255", 4) == 0) {
      msg = "Speech interrupted";
    }

    systemLog("ATP Error", msg, 3, 3);
    return false;
  }
  // 正常終了
  return true;
}
