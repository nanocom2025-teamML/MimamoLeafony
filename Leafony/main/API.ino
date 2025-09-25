bool checkIsOutside(){
  return getDataFromJson<bool>(sendRequestJson(HTTP_GET, "/api/device/is_outside"),"is_outside");
}

// 伝言があるか確認
bool checkNotices() {
  return getDataFromJson<bool>(sendRequestJson(HTTP_GET, "/api/device/notices"),"has_unread");
}

// 伝言の取得
String getMessage() {
  return getDataFromJson<String>(sendRequestJson(HTTP_PATCH, "/api/device/touch-single"), "content");
}


// 伝言(マイクのバイナリデータ)の送信
String sendMessage() {
  String body = sendFileMultipart("/api/device/audio-bin", AUDIO_FILE_NAME);
  if (!body) {
    return "tsuushin/era-.";  // エラー
  }
  String text = getDataFromJson<String>(body, "text");
  if (text == ".") {
    return "kikitoremasenndeshita.";
  }
  return text + "wo/soushin/shimashita.";
}