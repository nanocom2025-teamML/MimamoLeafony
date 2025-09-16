void countDown(const int &count) {
  countDown(count, 0, false);
  systemLog("Start!",2,1);
  chime(true);
}
void countDown(const int &count, const bool &con) { countDown(count, 0, con); }
void countDown(const int &max, const int &min) { countDown(max, min, false); }
void countDown(const int &max, const int &min, const bool &con) {
  for (int i = max; con ? (i >= min) : (i > min); i--) {
    systemLog("count",String(i),1,1);
    speak_num(i);
    delay(600);
  }
}