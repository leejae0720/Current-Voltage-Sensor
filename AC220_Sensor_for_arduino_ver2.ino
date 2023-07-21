#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <math.h>

#define CHANNEL_Current_1 A0
#define CHANNEL_Current_2 A1
#define CHANNEL_Current_3 A2

#define CHANNEL_Voltage_1 A3
#define CHANNEL_Voltage_2 A4
#define CHANNEL_Voltage_3 A5
/*
const int CHANNEL_Current_1 = A0;
const int CHANNEL_Current_2 = A1;
const int CHANNEL_Current_3 = A2;

const int CHANNEL_Voltage_1 = A3;
const int CHANNEL_Voltage_2 = A4;
const int CHANNEL_Voltage_3 = A5;
*/
bool print_flag = false;
unsigned int serial_flag = 0;

// timer interrupt setting
volatile unsigned int timer_count = 0;

// data distinct digit
int i = 0, j = 0;

int Current_data_arr0[500], Current_data_arr1[500], Current_data_arr2[500];
int Voltage_data_arr0[500], Voltage_data_arr1[500], Voltage_data_arr2[500];

// sin curve fitting value
int angle0[500], angle1[500], angle2[500], angle3[500], angle4[500], angle5[500];
int fitting_sampling_time0[500], fitting_sampling_time1[500], fitting_sampling_time2[500], fitting_sampling_time3[500], fitting_sampling_time4[500], fitting_sampling_time5[500];

// data variable
int Current0_sum = 0, Current0_B = 0, Current0_X = 0, Current0_theta = 0;
int Current0_A1[500], Current0_A2[500];

int Current1_sum = 0, Current1_B = 0, Current1_X = 0, Current1_theta = 0;
int Current1_A1[500], Current1_A2[500];

int Current2_sum = 0, Current2_B = 0, Current2_X = 0, Current2_theta = 0;
int Current2_A1[500], Current2_A2[500];

int Voltage0_sum = 0, Voltage0_B = 0, Voltage0_X = 0, Voltage0_theta = 0;
int Voltage0_A1[500], Voltage0_A2[500];

int Voltage1_sum = 0, Voltage1_B = 0, Voltage1_X = 0, Voltage1_theta = 0;
int Voltage1_A1[500], Voltage1_A2[500];

int Voltage2_sum = 0, Voltage2_B = 0, Voltage2_X = 0, Voltage2_theta = 0;
int Voltage2_A1[500], Voltage2_A2[500];

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
  Serial.begin(115200);
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
    for (j = 0; j < 500; j++){
      fitting_sampling_time0[j] = 0.001 * (j+1);
      angle0[j] = 2 * PI * 60 * fitting_sampling_time0[j];

      fitting_sampling_time1[j] = 0.001 * (j+1);
      angle1[j] = 2 * PI * 60 * fitting_sampling_time1[j];

      fitting_sampling_time2[j] = 0.001 * (j+1);
      angle2[j] = 2 * PI * 60 * fitting_sampling_time2[j];

      fitting_sampling_time3[j] = 0.001 * (j+1);
      angle3[j] = 2 * PI * 60 * fitting_sampling_time3[j];

      fitting_sampling_time4[j] = 0.001 * (j+1);
      angle4[j] = 2 * PI * 60 * fitting_sampling_time4[j];

      fitting_sampling_time5[j] = 0.001 * (j+1);
      angle5[j] = 2 * PI * 60 * fitting_sampling_time5[j];

      Current0_sum += Current_data_arr0[j];
      Current1_sum += Current_data_arr1[j];
      Current2_sum += Current_data_arr2[j];
      Voltage0_sum += Voltage_data_arr0[j];
      Voltage1_sum += Voltage_data_arr1[j];
      Voltage2_sum += Voltage_data_arr2[j];  
    }

    // Current 1
    Current0_B = (1/500)*Current0_sum;
    for(int a = 0; a < 500; a++){
      Current0_A1[a] = 2*Current0_B*sin(angle0[a]);
      Current0_A2[a] = 2*Current0_B*cos(angle0[a]);
      Current0_X = sqrt(sq(Current0_A1[a])+sq(Current0_A2[a]));
      Current0_theta = atan2(Current0_A2[a] ,Current0_A1[a]);
    }
    
    // Current 2
    Current1_B = (1/500)*Current1_sum;
    for(int b = 0; b < 500; b++){
      Current1_A1[b] = 2*Current1_B*sin(angle1[b]);
      Current1_A2[b] = 2*Current1_B*cos(angle1[b]);
      Current1_X = sqrt(sq(Current1_A1[b])+sq(Current1_A2[b]));
      Current1_theta = atan2(Current1_A2[b] ,Current1_A1[b]);
    }

    // Current 3
    Current2_B = (1/500)*Current2_sum;
    for(int c = 0; c < 500; c++){
      Current2_A1[c] = 2*Current2_B*sin(angle2[c]);
      Current2_A2[c] = 2*Current2_B*cos(angle2[c]);
      Current2_X = sqrt(sq(Current2_A1[c])+sq(Current2_A2[c]));
      Current2_theta = atan2(Current2_A2[c] ,Current2_A1[c]);
    }

    // Voltage 1
    Voltage0_B = (1/500)*Voltage0_sum;
    for(int d = 0; d < 500; d++){
      Voltage0_A1[d] = 2*Voltage0_B*sin(angle3[d]);
      Voltage0_A2[d] = 2*Voltage0_B*sin(angle3[d]);
      Voltage0_X = sqrt(sq(Voltage0_A1[d])+sq(Voltage0_A2[d]));
      Voltage0_theta = atan2(Voltage0_A2[d], Voltage0_A1[d]);
     }
    
    // Voltage 2
    Voltage1_B = (1/500)*Voltage1_sum;
    for(int e = 0; e < 500; e++){
      Voltage1_A1[e] = 2*Voltage1_B*sin(angle4[e]);
      Voltage1_A2[e] = 2*Voltage1_B*sin(angle4[e]);
      Voltage1_X = sqrt(sq(Voltage1_A1[e])+sq(Voltage1_A2[e]));
      Voltage1_theta = atan2(Voltage1_A2[e], Voltage1_A1[e]);
     }

    // Voltage 3
    Voltage2_B = (1/500)*Voltage2_sum;
    for(int f = 0; f < 500; f++){
      Voltage2_A1[f] = 2*Voltage2_B*sin(angle5[f]);
      Voltage2_A2[f] = 2*Voltage2_B*sin(angle5[f]);
      Voltage2_X = sqrt(sq(Voltage2_A1[f])+sq(Voltage2_A2[f]));
      Voltage2_theta = atan2(Voltage2_A2[f], Voltage2_A1[f]);
     }

    print_flag = false;
  }

  else if (timer_count == 7) { // after 9.5sec  // 20
    Current_data_arr0[500] = {};
    Current_data_arr1[500] = {};
    Current_data_arr2[500] = {};

    Voltage_data_arr0[500] = {};
    Voltage_data_arr1[500] = {};
    Voltage_data_arr2[500] = {};

    Current0_A1[500], Current0_A2[500], Current1_A1[500], Current1_A2[500], Current2_A1[500], Current2_A2[500] = {};
    Voltage0_A1[500], Voltage0_A2[500], Voltage1_A1[500], Voltage1_A2[500], Voltage2_A1[500], Voltage2_A2[500] = {};
    angle0[500], angle1[500], angle2[500], angle3[500], angle4[500], angle5[500] = {};
    fitting_sampling_time0[500], fitting_sampling_time1[500], fitting_sampling_time2[500], fitting_sampling_time3[500], fitting_sampling_time4[500], fitting_sampling_time5[500] = {};
    
    Current0_sum = 0, Current0_B = 0, Current0_X = 0, Current0_theta = 0;
    Current1_sum = 0, Current1_B = 0, Current1_X = 0, Current1_theta = 0;
    Current2_sum = 0, Current2_B = 0, Current2_X = 0, Current2_theta = 0;

    Voltage0_sum = 0, Voltage0_B = 0, Voltage0_X = 0, Voltage0_theta = 0;
    Voltage1_sum = 0, Voltage1_B = 0, Voltage1_X = 0, Voltage1_theta = 0;
    Voltage2_sum = 0, Voltage2_B = 0, Voltage2_X = 0, Voltage2_theta = 0;

    timer_count = 0;
  }

  else if (timer_count == 6) { // 19
    if (!print_flag) {
      /*
      Serial.print(Current0_B);
      Serial.print('\t');
      Serial.print(Current0_X);
      Serial.print('\t');
      Serial.print(Current0_theta);
      Serial.print('\t');
      Serial.print(Current1_B);
      Serial.print('\t');
      Serial.print(Current1_X);
      Serial.print('\t');
      Serial.print(Current1_theta);
      Serial.print('\t');
      Serial.print(Current2_B);
      Serial.print('\t');
      Serial.print(Current2_X);
      Serial.print('\t');
      Serial.print(Current2_theta);
      Serial.print('\t');
      Serial.print(Voltage0_B);
      Serial.print('\t');
      Serial.print(Voltage0_X);
      Serial.print('\t');
      Serial.print(Voltage0_theta);
      Serial.print('\t');
      Serial.print(Voltage1_B);
      Serial.print('\t');
      Serial.print(Voltage1_X);
      Serial.print('\t');
      Serial.print(Voltage1_theta);
      Serial.print('\t');
      Serial.print(Voltage2_B);
      Serial.print('\t');
      Serial.print(Voltage2_X);
      Serial.print('\t');
      Serial.println(Voltage2_theta);
      */

      Serial.println(Current0_sum);

      print_flag = true;
    }
  }
}
