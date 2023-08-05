#include <HardwareSerial.h>
#define CHANNEL_Current_1 13
#define CHANNEL_Current_2 12
#define CHANNEL_Current_3 14

#define CHANNEL_Voltage_1 27
#define CHANNEL_Voltage_2 26
#define CHANNEL_Voltage_3 25

bool print_flag = false;

//timer interrupt setting
hw_timer_t *timer0 = NULL;
hw_timer_t *timer1 = NULL;

unsigned int timer_count = 0;
int Current_Value_0, Current_Value_1, Current_Value_2;
int start0, end0, minVal0, maxVal0;
int start1, end1, minVal1, maxVal1;
int start2, end2, minVal2, maxVal2;

unsigned int serial_flag = 0;

// data distinct digit
int i= 0;

// Current variable
int a = 0, c = 0, e = 0;
int b, d, f;

// Voltage variable
int g = 0, l = 0, n = 0;
int h, m, o;

int Current_data_arr0[500], Current_data_arr1[500], Current_data_arr2[500];
int Voltage_data_arr0[500], Voltage_data_arr1[500], Voltage_data_arr2[500];
int Current_data_mmd0[10], Current_data_mmd1[10], Current_data_mmd2[10];
int Voltage_data_mmd0[10], Voltage_data_mmd1[10], Voltage_data_mmd2[10];

int Current_mmd_sum0 = 0, Current_mmd_sum1 = 0, Current_mmd_sum2 = 0;
int Current_count0 = 0, Current_count1 = 0, Current_count2 = 0;
int Current_average0 = 0, Current_average1 = 0, Current_average2 =0;

int Voltage_mmd_sum0 = 0, Voltage_mmd_sum1 = 0, Voltage_mmd_sum2 = 0;
int Voltage_count0 = 0, Voltage_count1 = 0, Voltage_count2 = 0;
int Voltage_average0 = 0, Voltage_average1 = 0, Voltage_average2 =0;

void IRAM_ATTR onTimer0() { // interrupt function
  if(timer_count == 0){
    Current_data_arr0[i] = analogRead(CHANNEL_Current_1);
    Current_data_arr1[i] = analogRead(CHANNEL_Current_2);
    Current_data_arr2[i] = analogRead(CHANNEL_Current_3);

    Voltage_data_arr0[i] = analogRead(CHANNEL_Voltage_1);
    Voltage_data_arr1[i] = analogRead(CHANNEL_Voltage_2);
    Voltage_data_arr2[i] = analogRead(CHANNEL_Voltage_3);

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
  
  else if(timer_count >= 1 && timer_count < 6.5){  // 19.5

    // a, b 변수 사용
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
      Current_mmd_sum0 += Current_data_mmd0[a];
      Current_count0++;
    }
    
    // c, d 변수 사용
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
      Current_mmd_sum1 += Current_data_mmd1[c];
      Current_count1++;
    }

    // e, f 변수 사용
    for(e = 0; e < 10; e++){
      start2 = e * 50;
      end2 = start2 + 49;
      minVal2 = Current_data_arr2[start2];
      maxVal2 = Current_data_arr2[start2];
      for(f = start2 + 1; f <= end2; f++){
        if(Current_data_arr2[f] < minVal2){
          minVal2 = Current_data_arr2[f];
        }
        if(Current_data_arr2[f] > maxVal2){
          maxVal2 = Current_data_arr2[f];
        }
      }
      Current_data_mmd2[e] = maxVal2 - minVal2;
      Current_mmd_sum2 += Current_data_mmd2[e];
      Current_count2++;
    }

    Current_average0 = Current_mmd_sum0 / Current_count0;
    Current_average1 = Current_mmd_sum1 / Current_count1;
    Current_average2 = Current_mmd_sum2 / Current_count2;
    print_flag = false; 
  }
  

  if(timer_count == 10){ // after 9.5sec  // 20
    
    Current_data_arr0[500] = {0, };
    Current_data_arr1[500] = {0, };
    Current_data_arr2[500] = {0, };
    
    Current_data_mmd0[10] = {0, };
    Current_data_mmd1[10] = {0, };
    Current_data_mmd2[10] = {0, };
   
    Current_count0= 0;
    Current_count1= 0;
    Current_average0 = 0;
    Current_average1 = 0;
    Current_mmd_sum0 = 0;
    Current_mmd_sum1 = 0;
    timer_count = 0;
  }

  else if(timer_count == 6){ // 19
    if(!print_flag){
      Serial.println(Current_average0);
      //Serial.print("\t");
      //Serial.print(Voltage_average0);
      //Serial.print(Current_average1);
      //Serial.print("\t");
      //Serial.println(Current_average2);
      print_flag = true;
    }
  }

  else if(timer_count > 1 && timer_count < 6){ // 19

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
      Current_mmd_sum0 += Current_data_mmd0[a];
      Current_count0++;
    }
    Current_average0 = Current_mmd_sum0 / Current_count0;

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
      Current_mmd_sum1 += Current_data_mmd1[c];
      Current_count1++;
    }
    Current_average1 = Current_mmd_sum1 / Current_count1;

    for(e = 0; e < 10; e++){
      start2 = e * 50;
      end2 = start2 + 49;
      minVal2 = Current_data_arr2[start2];
      maxVal2 = Current_data_arr2[start2];
      for(f = start2 + 1; f <= end2; f++){
        if(Current_data_arr2[f] < minVal2){
          minVal2 = Current_data_arr2[f];
        }
        if(Current_data_arr2[f] > maxVal2){
          maxVal2 = Current_data_arr2[f];
        }
      }
      Current_data_mmd2[e] = maxVal2 - minVal2;
      Current_mmd_sum2 += Current_data_mmd2[e];
      Current_count2++;
    }
    Current_average2 = Current_mmd_sum2 / Current_count2;


    print_flag = false;
  }  
}
