// デフォルト値を型ごとに返す構造体
template <typename T>
struct DefaultValue;  // 宣言だけ（未対応型を使ったらコンパイルエラーになる）
// 特殊化
template <>
struct DefaultValue<bool> {
  static bool value() { return false; }
};
template <>
struct DefaultValue<int> {
  static int value() { return 0; }
};
template <>
struct DefaultValue<float> {
  static float value() { return 0.0f; }
};
template <>
struct DefaultValue<String> {
  static String value() { return String(""); }
};

// JSONからキーに対応する値を抜き出す（String生成最小化）
const char *extractValue(const char *json, const char *key, char *buffer,
                         size_t bufsize) {
  const char *pos = strstr(json, key);
  if (!pos) {
    buffer[0] = '\0';
    return buffer;
  }
  // コロンを探す
  pos = strchr(pos, ':');
  if (!pos) {
    buffer[0] = '\0';
    return buffer;
  }
  pos++;  // コロンの次へ
  // 空白スキップ
  while (*pos && isspace(*pos)) pos++;
  // 文字列値の場合
  if (*pos == '\"') {
    pos++;
    size_t i = 0;
    while (*pos && *pos != '\"' && i < bufsize - 1) buffer[i++] = *pos++;
    buffer[i] = '\0';
    return buffer;
  }
  // 数値や true/false の場合
  size_t i = 0;
  while (*pos && *pos != ',' && *pos != '}' && i < bufsize - 1) {
    buffer[i++] = *pos++;
  }
  buffer[i] = '\0';
  return buffer;
}

// 型変換ラッパ

template <typename T>
T getDataFromJson(const char *json, const char *key) {
  return getDataFromJson(json, key, DefaultValue<T>::value());
}

template <typename T>
T getDataFromJson(const char *json, const char *key, const T &defaultValue) {
  char buf[64];
  extractValue(json, key, buf, sizeof(buf));
  return buf[0] ? String(buf) : defaultValue;
}

// ======== String 対応オーバーロード ========
template <typename T>
T getDataFromJson(const String &json, const char *key) {
  return getDataFromJson(json.c_str(), key, DefaultValue<T>::value());
}

template <typename T>
T getDataFromJson(const String &json, const char *key, const T &defaultValue) {
  return getDataFromJson(json.c_str(), key, defaultValue);
}

// 文字列
template <>
String getDataFromJson<String>(const char *json, const char *key,
                               const String &defaultValue) {
  char buf[64];
  extractValue(json, key, buf, sizeof(buf));
  return buf[0] ? String(buf) : defaultValue;
}

// 整数
template <>
int getDataFromJson<int>(const char *json, const char *key,
                         const int &defaultValue) {
  char buf[32];
  extractValue(json, key, buf, sizeof(buf));
  return buf[0] ? atoi(buf) : defaultValue;
}

// 小数
template <>
float getDataFromJson<float>(const char *json, const char *key,
                             const float &defaultValue) {
  char buf[32];
  extractValue(json, key, buf, sizeof(buf));
  return buf[0] ? atof(buf) : defaultValue;
}

// 真理値
template <>
bool getDataFromJson<bool>(const char *json, const char *key,
                           const bool &defaultValue) {
  char buf[6];  // "true" or "false"
  extractValue(json, key, buf, sizeof(buf));
  if (buf[0] == 't') return true;
  if (buf[0] == 'f') return false;
  return defaultValue;
}

// template <typename T>
// T getDataFromJson(const String &jsonString, const char *key) {
//   return getDataFromJson(jsonString, key, DefaultValue<T>::value());
// }
// template <typename T>
// T getDataFromJson(const String &jsonString, const char *key,const T
// &defaultValue) {
//   StaticJsonDocument<1024> doc;  // サイズはレスポンスに応じて調整
//   DeserializationError error = deserializeJson(doc, jsonString);
//   if (error) {
//     systemLog("deserializeJson() failed", error.f_str(), 3, 3);
//     return defaultValue;
//   }
//   // 指定キーが存在していて型一致なら値を返す
//   if (doc.containsKey(key) && doc[key].is<T>()) {
//     systemLog("Found", 2, 1);
//     return doc[key];
//   }
//   systemLog("Not Found", 3, 3);
//   return defaultValue;
// }

// // 配列の中のデータを番号指定で取得
// template <typename T>
// T getDataFromJsonArray(const String &jsonString, const char *arrayKey,const
// char *valueKey) {
//   getDataFromJsonArray(jsonString, arrayKey, valueKey,
//   0,DefaultValue<T>::value());
// }

// template <typename T>
// T getDataFromJsonArray(const String &jsonString, const char *arrayKey,const
// char *valueKey, size_t index) {
//   getDataFromJsonArray(jsonString, arrayKey, valueKey,
//   index,DefaultValue<T>::value());
// }

// template <typename T>
// T getDataFromJsonArray(const String &jsonString, const char *arrayKey,const
// char *valueKey, size_t index,const T &defaultValue) {
//   StaticJsonDocument<1024> doc;  // 配列が大きい場合はサイズ調整
//   DeserializationError error = deserializeJson(doc, jsonString);

//   if (error) {
//     systemLog("deserializeJson() failed", error.f_str(), 3, 3);
//     return defaultValue;
//   }

//   if (!doc.containsKey(arrayKey)) {
//     systemLog("Array key not found", 3, 3);
//     return defaultValue;
//   }

//   JsonArray arr = doc[arrayKey].as<JsonArray>();
//   if (index >= arr.size()) {
//     systemLog("Index out of bounds", 3, 3);
//     return defaultValue;
//   }

//   JsonObject obj = arr[index].as<JsonObject>();
//   if (!obj.containsKey(valueKey)) {
//     systemLog("Value key not found", 3, 3);
//     return defaultValue;
//   }

//   return obj[valueKey].as<T>();
// }
