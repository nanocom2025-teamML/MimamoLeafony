#define COUNT_INTERVAL 1000UL

void countDown(int count) { countDown(count, 0, false); }
void countDown(int count, bool con) { countDown(count, 0, con); }
void countDown(int max, int min) { countDown(max, min, false); }

void countDown(int max, int min, bool con) {
  for (int i = max; con ? (i >= min) : (i > min); i--) {
    unsigned long next_time = millis() + COUNT_INTERVAL;
    systemLog("count", String(i), 1, 1);
    speak_num(i);
    long remain = (long)(next_time - millis());
    if (remain > 0) delay((unsigned long)remain);
  }
}
