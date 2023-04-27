#define CHANNEL_Current_1 36
#define CHANNEL_Current_2 39
#define CHANNEL_Current_3 34

// Get current time setting
/*
#include <WiFi.h>
#include <time.h>
const char* ssid = "IRMS";
const char* password = "0312012945";
*/

volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t *timer0 = NULL;
hw_timer_t *timer1 = NULL;

static long analogPinTimer = 0;
unsigned long thisMillis_old = 0;
unsigned long pastMillis = 0;
unsigned long deltaMillis = 0;
unsigned int timer_count = 0;
int Current_Value_0, Current_Value_1, Current_Value_2;
int start1, end1, minVal1, maxVal1;

unsigned int serial_flag = 0;
unsigned int switchflag = 0;  // 초기 상태
unsigned int interrupt_flag = 0;
unsigned int timerflag = 0;

int i= 0, j=0, k = 0; // 데이터 구별 숫자
int a = 0;
int b;

int reset_arr = {0, };
int Current_data_arr0[500], Current_data_arr1[500], Current_data_arr2[500];
int Current_data_mmd0[10], Current_data_mmd1[10], Current_data_mmd2[10];
int mmd_sum0 = 0;
int mmd_sum1 = 0;
int mmd_sum2 = 0;
int count0 = 0;
int count1 = 0;
int count2 = 0;
int average1 = 0;
int average2 = 0;


void IRAM_ATTR onTimer0() { // interrupt function
  if(timer_count == 0){
    //Current_data_arr0[i] = analogRead(CHANNEL_Current_1);
    Current_data_arr1[i] = analogRead(CHANNEL_Current_2);
    //Current_Value_2 = analogRead(CHANNEL_Current_3);

    i++;
  }
}

void IRAM_ATTR onTimer1() {
  timer_count++;
    
}


void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  //Sampling timer
  timer0 = timerBegin(0, 80, true);  // run for 0.1msec(0.0001sec) second
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

  //Serial.println(timer_count);

  if(timer_count != 0){
    i = 0;
  }
  
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
  
  if(timer_count == 1){  // 0.5sec ~ 9.5sec
    
    // Channel 2
    for(a =0; a < 10; a++){
      start1 = a * 50;
      end1 = start1 + 49;
      minVal1 = Current_data_arr1[start1];
      maxVal1 = Current_data_arr1[start1];
      for(b = start1 + 1; b <= end1; b++){
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
    average1 = mmd_sum1 / count1;
  }
  */
  
  if(timer_count == 4){ // after 9.5sec
    
    Serial.println(average2);

    //Current_data_arr0[500] = {0, };
    Current_data_arr1[500] = {0, };
    //Current_data_arr2[500] = {0, };

    //Current_data_mmd0[10] = {0, };
    Current_data_mmd1[10] = {0, };
    //Current_data_mmd2[10] = {0, };
    
    count0= 0;
    count1= 0;
    count2 = 0;
    mmd_sum0 = 0;
    mmd_sum1 = 0;
    mmd_sum2 = 0;
    timer_count = 0;
  }
  else{ // 0.5sec ~ 9.5sec
    for(a =0; a < 10; a++){
      start1 = a * 50;
      end1 = start1 + 49;
      minVal1 = Current_data_arr1[start1];
      maxVal1 = Current_data_arr1[start1];
      for(b = start1 + 1; b <= end1; b++){
        if(Current_data_arr1[b] < minVal1){
          minVal1 = Current_data_arr1[b];
        }
        if(Current_data_arr1[b] > maxVal1){
          maxVal1 = Current_data_arr1[b];
        }
      }
      //Serial.println(minVal1);
      //Serial.println(maxVal1);
      Current_data_mmd1[a] = maxVal1 - minVal1;
      mmd_sum1 += Current_data_mmd1[a];
      count1++;
      //Serial.println(count1);
      //Serial.println(mmd_sum1);
    }
    average1 = mmd_sum1 / count1;
    //Serial.println(average1);
    average2 = average1;
  }
  //Serial.println(Current_data_arr1[i]);
}