// ESP32 pin test code
/*
//const int analogInPin = 14; // 비접촉 전류 센서가 연결된 아날로그 입력 핀
const int analogInPin = 32
int sensorValue = 0;       // 센서 값 저장 변수

void setup() {
  Serial.begin(115200);     // 시리얼 통신 시작
}

void loop() {
  sensorValue = analogRead(analogInPin);    // 아날로그 입력값 읽기
  Serial.println(sensorValue);              // 샘플링 값 출력
  delay(100);                                // 10ms 딜레이
}
*/


// interrupt test code
/*
#include <MsTimer2.h>

const int NUM_SAMPLES = 500;  // 샘플 개수
int sensorValue[NUM_SAMPLES];
int currentIndex = 0;
unsigned long sensorValue_sum = 0;

void onTimer0() {
  sensorValue[currentIndex] = analogRead(A0);
  currentIndex = (currentIndex + 1) % NUM_SAMPLES;  // 다음 인덱스로 이동
}

void setup() {
  Serial.begin(115200); // 시리얼 통신을 위한 초기화
  MsTimer2::set(1, onTimer0);
  MsTimer2::start();
}

void loop() {
  if (currentIndex == 0) {  // 500개의 샘플을 모두 읽은 경우
    sensorValue_sum = 0;  // 총 합 초기화

    for (int i = 0; i < NUM_SAMPLES; i++) {
      sensorValue_sum += sensorValue[i];  // 총 합 계산
    }

    Serial.println(sensorValue_sum); // 시리얼 모니터에 값 출력
  }
}
*/

// arduino pin test code
void setup() {
  Serial.begin(115200); // 시리얼 통신을 위한 초기화
}

void loop() {
  int sensorValue = analogRead(A0); // A0 핀에서 아날로그 값 읽기
  Serial.println(sensorValue);              // 샘플링 값 출력
  delay(1);
}
