#include <WiFi.h>
#include <time.h>

const char* ssid = "IRMS";
const char* password = "0312012945";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  configTime(9 * 3600, 0, "pool.ntp.org"); // 서울 기준, NTP 서버로부터 시간 정보를 받아옵니다.
}

void loop() {
  static unsigned long prev_time = 0; // 이전 시간을 저장하는 변수
  unsigned long current_time = millis(); // 현재 시간을 측정하는 변수
  
  if (current_time - prev_time >= 10000) { // 현재 시간과 이전 시간을 비교하여 10초 이상 지났으면
    prev_time = current_time; // 이전 시간을 현재 시간으로 업데이트
    struct tm timeinfo; // 현재 시간 정보를 저장할 tm 구조체
    if(!getLocalTime(&timeinfo)){ // NTP 서버로부터 시간 정보를 받아옵니다.
      Serial.println("Failed to obtain time");
      return;
    }
    Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec); // 년월일시분초를 출력합니다.
  }
}
