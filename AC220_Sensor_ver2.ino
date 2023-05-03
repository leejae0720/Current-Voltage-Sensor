#include <HardwareSerial.h>
#define CHANNEL_Current_1 34
#define CHANNEL_Current_2 39
//#define CHANNEL_Current_3 34

// 36 39 34

bool print_flag = false;

//timer interrupt setting
hw_timer_t *timer0 = NULL;
hw_timer_t *timer1 = NULL;

unsigned int timer_count = 0;
int Current_Value_0, Current_Value_1;
int start0, end0, minVal0, maxVal0;
int start1, end1, minVal1, maxVal1;

unsigned int serial_flag = 0;

// data distinct digit
int i= 0;
int a = 0, c = 0;
int b, d;

int Current_data_arr0[500], Current_data_arr1[500];
int Current_data_mmd0[10], Current_data_mmd1[10];
int mmd_sum0 = 0;
int mmd_sum1 = 0;
int count0 = 0;
int count1 = 0;
int average0 = 0;
int average1 = 0;


void IRAM_ATTR onTimer0() { // interrupt function
  if(timer_count == 0){
    Current_data_arr0[i] = analogRead(CHANNEL_Current_1);
    Current_data_arr1[i] = analogRead(CHANNEL_Current_2);

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
}

void loop() {

  if(timer_count != 0){
    i = 0;
  }
  
  
  else if(timer_count >= 1 && timer_count < 19.5){

    for(a =0; a < 10; a++){
      start0 = a * 50;
      end0 = start0 + 49;
      minVal0 = Current_data_arr0[start0];
      maxVal0 = Current_data_arr0[start0];
      for(b = start0 + 1; b <= end1; b++){
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
    
    for(c =0; c < 10; c++){
      start1 = c * 50;
      end1 = start1 + 49;
      minVal1 = Current_data_arr1[start1];
      maxVal1 = Current_data_arr1[start1];
      for(d = start1 + 1; d <= end1; d++){
        if(Current_data_arr1[d] < minVal1){
          minVal1 = Current_data_arr1[d];
        }
        if(Current_data_arr1[d] > maxVal1){
          maxVal1 = Current_data_arr1[d];
        }
      }
      Current_data_mmd1[c] = maxVal1 - minVal1;
      mmd_sum1 += Current_data_mmd1[c];
      count1++;
    }

    average0 = mmd_sum0 / count0;
    average1 = mmd_sum1 / count1;
    print_flag = false; 
  }
  

  if(timer_count == 20){ // after 9.5sec
    
    Current_data_arr0[500] = {0, };
    Current_data_arr1[500] = {0, };
    
    Current_data_mmd0[10] = {0, };
    Current_data_mmd1[10] = {0, };
   
    count0= 0;
    count1= 0;
    average0 = 0;
    average1 = 0;
    mmd_sum0 = 0;
    mmd_sum1 = 0;
    timer_count = 0;
  }

  else if(timer_count == 19){
    if(!print_flag){
      Serial.print(average0);
      Serial.print(",");
      Serial.println(average1);
      print_flag = true;
    }
  }

  else if(timer_count > 1 && timer_count < 19){

    for(a =0; a < 10; a++){
      start0 = a * 50;
      end0 = start0 + 49;
      minVal0 = Current_data_arr0[start0];
      maxVal0 = Current_data_arr0[start0];
      for(b = start0 + 1; b <= end1; b++){
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
    average0 = mmd_sum0 / count0;

    for(c =0; c < 10; c++){
      start1 = c * 50;
      end1 = start1 + 49;
      minVal1 = Current_data_arr1[start1];
      maxVal1 = Current_data_arr1[start1];
      for(d = start1 + 1; d <= end1; d++){
        if(Current_data_arr1[d] < minVal1){
          minVal1 = Current_data_arr1[d];
        }
        if(Current_data_arr1[d] > maxVal1){
          maxVal1 = Current_data_arr1[d];
        }
      }
      Current_data_mmd1[c] = maxVal1 - minVal1;
      mmd_sum1 += Current_data_mmd1[c];
      count1++;
    }

    average1 = mmd_sum1 / count1;
    print_flag = false;
  }  
}