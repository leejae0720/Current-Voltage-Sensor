#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <math.h>

#define CHANNEL_Current_1 A0
#define CHANNEL_Current_2 A1
#define CHANNEL_Current_3 A2

#define CHANNEL_Voltage_1 A3
#define CHANNEL_Voltage_2 A4
#define CHANNEL_Voltage_3 A5

bool print_flag = false;

// timer interrupt setting
volatile unsigned int timer_count = 0;

// current value
int Current_Value_0, Current_Value_1, Current_Value_2;

// current value
int start0, end0, minVal0, maxVal0;
int start1, end1, minVal1, maxVal1;
int start2, end2, minVal2, maxVal2;

// voltage value
int start3, end3, minVal3, maxVal3;
int start4, end4, minVal4, maxVal4;
int start5, end5, minVal5, maxVal5;

unsigned int serial_flag = 0;

// data distinct digit
int i = 0;

// Current variable
int a = 0, c = 0, e = 0;
int b, d, f;

// Voltage variable
int g = 0, l = 0, n = 0;
int h, m, o;

int Current_data_arr0[500], Current_data_arr1[500], Current_data_arr2[500];
int Current_data_mmd0[10], Current_data_mmd1[10], Current_data_mmd2[10];
int Voltage_data_arr0[500], Voltage_data_arr1[500], Voltage_data_arr2[500];
int Voltage_data_mmd0[10], Voltage_data_mmd1[10], Voltage_data_mmd2[10];

int Current_sum0 = 0, Current_sum1 = 0, Current_sum2 = 0;
int Current_mmd_sum0 = 0, Current_mmd_sum1 = 0, Current_mmd_sum2 = 0;
int Current_count0 = 0, Current_count1 = 0, Current_count2 = 0;
int Current_average0 = 0, Current_average1 = 0, Current_average2 = 0;

int Voltage_mmd_sum0 = 0, Voltage_mmd_sum1 = 0, Voltage_mmd_sum2 = 0;
int Voltage_count0 = 0, Voltage_count1 = 0, Voltage_count2 = 0;
int Voltage_average0 = 0, Voltage_average1 = 0, Voltage_average2 = 0;

// sin curve fitting value
float t;
float angle;

// data variable
float Current0_sum = 0, Current0_A1 = 0, Current0_A2 = 0, Current0_B = 0, Current0_X = 0, Current0_theta = 0;
float Current1_sum = 0, Current1_A1 = 0, Current1_A2 = 0, Current1_B = 0, Current1_X = 0, Current1_theta = 0;
float Current2_sum = 0, Current2_A1 = 0, Current2_A2 = 0, Current2_B = 0, Current2_X = 0, Current2_theta = 0;

float Voltage0_sum = 0, Voltage0_A1 = 0, Voltage0_A2 = 0, Voltage0_B = 0, Voltage0_X = 0, Voltage0_theta = 0;
float Voltage1_sum = 0, Voltage1_A1 = 0, Voltage1_A2 = 0, Voltage1_B = 0, Voltage1_X = 0, Voltage1_theta = 0;
float Voltage2_sum = 0, Voltage2_A1 = 0, Voltage2_A2 = 0, Voltage2_B = 0, Voltage2_X = 0, Voltage2_theta = 0;

//SoftwareSerial SerialMega(10, 11);  // 아두이노 메가의 시리얼 통신을 위한 SoftwareSerial 설정

void onTimer0() { // interrupt function
  if (timer_count == 0) {
    Current_data_arr0[i] = analogRead(CHANNEL_Current_1);
    Current_data_arr1[i] = analogRead(CHANNEL_Current_2);
    Current_data_arr2[i] = analogRead(CHANNEL_Current_3);

    Voltage_data_arr0[i] = analogRead(CHANNEL_Voltage_1);
    Voltage_data_arr1[i] = analogRead(CHANNEL_Voltage_2);
    Voltage_data_arr2[i] = analogRead(CHANNEL_Voltage_3);

    i++;
  }
}

void onTimer1() {
  timer_count++;
}

void setup() {
  Serial.begin(115200); // 아두이노 메가와의 시리얼 통신을 위한 속도 설정
  pinMode(2, OUTPUT);

  MsTimer2::set(1, onTimer0); // 0.001 sec Sampling time
  MsTimer2::set(500, onTimer1); // 0.5 sec Timer (Mode Change timer)
  MsTimer2::start();

}

void loop() {
  if (timer_count != 0) {
    i = 0;
  }

  if (timer_count >= 1 && timer_count < 5.5) {  // 19.5

    // sin curve fitting function
    for (a = 0; a < 500; a++){
      t = 0.001 * (a+1);
      angle = 2 * PI * 60 * t;

      Current0_sum += Current_data_arr0[a];
      Current1_sum += Current_data_arr1[a];
      Current2_sum += Current_data_arr2[a];
      Voltage0_sum += Voltage_data_arr0[a];
      Voltage1_sum += Voltage_data_arr1[a];
      Voltage2_sum += Voltage_data_arr2[a];
      
    }

    // Current 1
    Current0_B = (1/500)*Current0_sum;
    Current0_A1 = 2*Current0_B*sin(angle);
    Current0_A2 = 2*Current0_B*cos(angle);
    Current0_X = sqrt(sq(Current0_A1)+sq(Current0_A2));
    Current0_theta = atan2(Current0_A2 ,Current0_A1);

    // Current 2
    Current1_B = (1/500)*Current1_sum;
    Current1_A1 = 2*Current1_B*sin(angle);
    Current1_A2 = 2*Current1_B*cos(angle);
    Current1_X = sqrt(sq(Current1_A1)+sq(Current1_A2));
    Current1_theta = atan2(Current1_A2 ,Current1_A1);

    // Current 3
    Current2_B = (1/500)*Current2_sum;
    Current2_A1 = 2*Current2_B*sin(angle);
    Current2_A2 = 2*Current2_B*cos(angle);
    Current2_X = sqrt(sq(Current2_A1)+sq(Current2_A2));
    Current2_theta = atan2(Current2_A2 ,Current2_A1);

    // Voltage 1
    Voltage0_B = (1/500)*Voltage0_sum;
    Voltage0_A1 = 2*Voltage0_B*sin(angle);
    Voltage0_A2 = 2*Voltage0_B*cos(angle);
    Voltage0_X = sqrt(sq(Voltage0_A1)+sq(Voltage0_A2));
    Voltage0_theta = atan2(Voltage0_A2 ,Voltage0_A1);

    // Voltage 2
    Voltage1_B = (1/500)*Voltage1_sum;
    Voltage1_A1 = 2*Voltage1_B*sin(angle);
    Voltage1_A2 = 2*Voltage1_B*cos(angle);
    Voltage1_X = sqrt(sq(Voltage1_A1)+sq(Voltage1_A2));
    Voltage1_theta = atan2(Voltage1_A2 ,Voltage1_A1);

    // Voltage 3
    Voltage2_B = (1/500)*Voltage2_sum;
    Voltage2_A1 = 2*Voltage2_B*sin(angle);
    Voltage2_A2 = 2*Voltage2_B*cos(angle);
    Voltage2_X = sqrt(sq(Voltage2_A1)+sq(Voltage2_A2));
    Voltage2_theta = atan2(Voltage2_A2 ,Voltage2_A1);

    print_flag = false;
  }

  else if (timer_count == 7) { // after 9.5sec  // 20
    Current_data_arr0[500] = { };
    Current_data_arr1[500] = { };
    Current_data_arr2[500] = { };

    Voltage_data_arr0[500] = { };
    Voltage_data_arr1[500] = { };
    Voltage_data_arr2[500] = { };

    Current_count0 = 0;
    Current_count1 = 0;
    Current_count2 = 0;
    Current_average0 = 0;
    Current_average1 = 0;
    Current_average2 = 0;
    Current_mmd_sum0 = 0;
    Current_mmd_sum1 = 0;
    Current_mmd_sum2 = 0;

    Voltage_count0 = 0;
    Voltage_count1 = 0;
    Voltage_count2 = 0;
    Voltage_average0 = 0;
    Voltage_average1 = 0;
    Voltage_average2 = 0;
    Voltage_mmd_sum0 = 0;
    Voltage_mmd_sum1 = 0;
    Voltage_mmd_sum2 = 0;

    timer_count = 0;
  }

  else if (timer_count == 6) { // 19
    if (!print_flag) {
      Serial.print(Current0_B);
      Serial.print('\t');
      Serial.print(Current0_X);
      Serial.print('\t');
      Serial.print(Current0_theta);
      
      
      print_flag = true;
    }
  }
}
