#define CHANNEL_Current_1 36
#define CHANNEL_Current_2 39
#define CHANNEL_Current_3 34
#define CHANNEL_Voltage_1 35
#define CHANNEL_Voltage_2 32
#define CHANNEL_Voltage_3 33

// Get current time setting
#include <WiFi.h>
#include <time.h>
const char* ssid = "IRMS";
const char* password = "0312012945";

volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t *timer0 = NULL;
hw_timer_t *timer1 = NULL;

volatile boolean sampling_timer = true;

static long analogPinTimer = 0;
unsigned long thisMillis_old;
unsigned long pastMillis = 0;
unsigned long deltaMillis = 0;
unsigned int timer_count = 0;
int Current_Value_0, Current_Value_1, Current_Value_2;
int Voltage_Value_1, Voltage_Value_2, Voltage_Value_3;

unsigned int serial_flag = 0;
unsigned int switchflag = 0;  // 초기 상태
unsigned int interrupt_flag = 0;
unsigned int timerflag = 0;

int i, j, k = 0; // 데이터 구별 숫자

int Current_data_arr0[1000], Current_data_arr1[1000], Current_data_arr2[1000];
int Current_data_mmd0[10], Current_data_mmd1[10], Current_data_mmd2[10];
int mmd_sum0, mmd_sum1, mmd_sum2 = 0;
int count0, count1, count2 = 0;

volatile boolean gLedState = LOW;

void IRAM_ATTR onTimer0() { // interrupt function
  if(sampling_timer){
    Current_Value_0 = analogRead(CHANNEL_Current_1);
    Current_Value_1 = analogRead(CHANNEL_Current_2);
    Current_Value_2 = analogRead(CHANNEL_Current_3);
      
    Current_data_arr0[i] = Current_Value_0;
    Current_data_arr1[j] = Current_Value_1;
    Current_data_arr2[k] = Current_Value_2;

    gLedState = !gLedState;
    digitalWrite(2, gLedState);
  }
}

void IRAM_ATTR onTimer1() {
  timer_count += 1;

  if(serial_flag == 0){
      serial_flag = 1;
    }
}


void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  //Sampling timer
  timer0 = timerBegin(0, 80, true);  // run for 0.1msec second
  timerAttachInterrupt(timer0, &onTimer0, true);
  timerAlarmWrite(timer0, 1000, true); // 1msec Sampling time
  timerAlarmEnable(timer0);

  //mode change timer
  timer1 = timerBegin(1, 80, true);  // run for 0.5sec second
  timerAttachInterrupt(timer1, &onTimer1, true);
  timerAlarmWrite(timer1, 500000, true); 
  timerAlarmEnable(timer1);

  /*
  //For get current time wifi setting
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  configTime(9 * 3600, 0, "pool.ntp.org"); // Get seoul time from NTP server
  */
}

void loop() {
  static unsigned long prev_time = 0;
  unsigned long current_time = millis();

  /*
  if (current_time - prev_time >= 10000) { 
    prev_time = current_time; 
    struct tm timeinfo; 
    if(!getLocalTime(&timeinfo)){ 
      Serial.println("Failed to obtain time");
      return;
    }
    Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec); // 년월일시분초를 출력합니다.
  }
  */

  if(timer_count >= 1 && timer_count < 20){  // 0.5sec ~ 9.5sec
    
    sampling_timer = false; 
    /*
    // Channel 1
    for(int a =0; a < 10; a++){
      int start0 = a * 50;
      int end0 = start0 + 49;
      int minVal0 = Current_data_arr0[start0];
      int maxVal0 = Current_data_arr0[start0];
      for(int b = start0 + 1; b <= end0; b++){
        if(Current_data_arr0[b] < minVal0){
          minVal0 = Current_data_arr0[b];
        }
        if(Current_data_arr0[b] > maxVal0){
          maxVal0 = Current_data_arr0[b];
        }
      }
      Current_data_mmd0[a] = maxVal0 - minVal0;
      mmd_sum0 += Current_data_mmd0[a];
      count0++;
    }
    float average0 = (float)mmd_sum0 / count0;
    Serial.println(average0);
    */
    // Channel 2
    for(int a =0; a < 10; a++){
      int start1 = a * 50;
      int end1 = start1 + 49;
      int minVal1 = Current_data_arr1[start1];
      int maxVal1 = Current_data_arr1[start1];
      for(int b = start1 + 1; b <= end1; b++){
        if(Current_data_arr1[b] < minVal1){
          minVal1 = Current_data_arr1[b];
        }
        if(Current_data_arr1[b] > maxVal1){
          maxVal1 = Current_data_arr1[b];
        }
      }
      Current_data_mmd1[a] = maxVal1 - minVal1;
      mmd_sum1 += Current_data_mmd1[a];
      count1++;
    }
    int average1 = mmd_sum1 / count1;

    if(serial_flag == 1){
      Serial.println(average1);
      serial_flag = 0;
    }

    /*
    // Channel 3
    for(int a =0; a < 10; a++){
      int start2 = a * 50;
      int end2 = start2 + 49;
      int minVal2 = Current_data_arr2[start2];
      int maxVal2 = Current_data_arr2[start2];
      for(int b = start2 + 1; b <= end2; b++){
        if(Current_data_arr2[b] < minVal2){
          minVal2 = Current_data_arr2[b];
        }
        if(Current_data_arr2[b] > maxVal2){
          maxVal2 = Current_data_arr2[b];
        }
      }
      Current_data_mmd2[a] = maxVal2 - minVal2;
      mmd_sum2 += Current_data_mmd2[a];
      count2++;
    }
    float average2 = (float)mmd_sum2 / count2;
    Serial.println(average2);
    */
  }
  else if(timer_count >= 20){ // after 9.5sec

    
    for(int c = 0; c < 1000; c++){
      //Current_data_arr0[c] = 0;
      Current_data_arr1[c] = 0;
      //Current_data_arr2[c] = 0;
    }
    
    for(int d = 0; d < 20; d++){
      //Current_data_mmd0[d] = 0;
      Current_data_mmd1[d] = 0;
      //Current_data_mmd2[d] = 0;
    }
    
    count0, count1, count2 = 0;
    mmd_sum0, mmd_sum1, mmd_sum2 = 0;

    sampling_timer = true;
    timer_count = 0;
  }
}