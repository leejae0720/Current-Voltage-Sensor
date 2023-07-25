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
unsigned int serial_flag = 0;

// timer interrupt setting
unsigned int timer_count = 0;
int count = 0;
// data distinct digit
int i = 0, j = 0;

unsigned int Current_data_arr0[500], Current_data_arr1[500], Current_data_arr2[500];
unsigned int Voltage_data_arr0[500], Voltage_data_arr1[500], Voltage_data_arr2[500];

// sin curve fitting value
int angle[500];
int fitting_sampling_time[500];

// data variable
int Current0_theta = 0;
volatile unsigned long Current0_sum = 0, Current0_B = 0, Current0_X = 0;
unsigned int Current0_A1[500], Current0_A2[500];

int Current1_theta = 0;
volatile unsigned long Current1_sum = 0, Current1_B = 0, Current1_X = 0;
unsigned int Current1_A1[500], Current1_A2[500];

int Current2_theta = 0;
volatile unsigned long Current2_sum = 0, Current2_B = 0, Current2_X = 0;
unsigned int Current2_A1[500], Current2_A2[500];

int Voltage0_theta = 0;
volatile unsigned long Voltage0_sum = 0, Voltage0_B = 0, Voltage0_X = 0;
unsigned int Voltage0_A1[500], Voltage0_A2[500];

int Voltage1_theta = 0;
volatile unsigned long Voltage1_sum = 0, Voltage1_B = 0, Voltage1_X = 0;
unsigned int Voltage1_A1[500], Voltage1_A2[500];

int Voltage2_theta = 0;
volatile unsigned long Voltage2_sum = 0, Voltage2_B = 0, Voltage2_X = 0;
unsigned int Voltage2_A1[500], Voltage2_A2[500];

void onTimer0() { // interrupt function
  if (timer_count >= 0 && timer_count < 500){  // 0 ~ 0.5 sec
    noInterrupts();
    Current_data_arr0[i] = analogRead(CHANNEL_Current_1);
    Current_data_arr1[i] = analogRead(CHANNEL_Current_2);
    Current_data_arr2[i] = analogRead(CHANNEL_Current_3);

    Voltage_data_arr0[i] = analogRead(CHANNEL_Voltage_1);
    Voltage_data_arr1[i] = analogRead(CHANNEL_Voltage_2);
    Voltage_data_arr2[i] = analogRead(CHANNEL_Voltage_3);

    i++;
    interrupts();
  }
  
  timer_count ++; 
}

void setup() {
  Serial.begin(115200);

  MsTimer2::set(1, onTimer0); // 0.001 sec Timer
  MsTimer2::start();
  for(int t = 0; t < 500; t++){
    fitting_sampling_time[j] = 0.001 * (j+1);
    angle[j] = 2 * PI * 60 * fitting_sampling_time[j];
  }
}

void loop() {
  if (timer_count == 1000) {  // 19.5
    
    // sin curve fitting function
    for (j = 0; j < 500; j++){
      
      noInterrupts();
      Current0_sum += Current_data_arr0[j];
      Current1_sum += Current_data_arr1[j];
      Current2_sum += Current_data_arr2[j];
      Voltage0_sum += Voltage_data_arr0[j];
      Voltage1_sum += Voltage_data_arr1[j];
      Voltage2_sum += Voltage_data_arr2[j]; 
      interrupts();
    
    }

    // Current 1
    Current0_B = Current0_sum/500;
    for(int a = 0; a < 500; a++){
      Current0_A1[a] = 2*Current0_B*sin(angle[a]);
      Current0_A2[a] = 2*Current0_B*cos(angle[a]);
      Current0_X = sqrt(sq(Current0_A1[a])+sq(Current0_A2[a]));
      Current0_theta = atan2(Current0_A2[a] ,Current0_A1[a]);
    }
    
    // Current 2
    Current1_B = Current1_sum/500;
    for(int b = 0; b < 500; b++){
      Current1_A1[b] = 2*Current1_B*sin(angle[b]);
      Current1_A2[b] = 2*Current1_B*cos(angle[b]);
      Current1_X = sqrt(sq(Current1_A1[b])+sq(Current1_A2[b]));
      Current1_theta = atan2(Current1_A2[b] ,Current1_A1[b]);
    }

    // Current 3
    Current2_B = Current2_sum/500;
    for(int c = 0; c < 500; c++){
      Current2_A1[c] = 2*Current2_B*sin(angle[c]);
      Current2_A2[c] = 2*Current2_B*cos(angle[c]);
      Current2_X = sqrt(sq(Current2_A1[c])+sq(Current2_A2[c]));
      Current2_theta = atan2(Current2_A2[c] ,Current2_A1[c]);
    }

    // Voltage 1
    Voltage0_B = Voltage0_sum/500;
    for(int d = 0; d < 500; d++){
      Voltage0_A1[d] = 2*Voltage0_B*sin(angle[d]);
      Voltage0_A2[d] = 2*Voltage0_B*sin(angle[d]);
      Voltage0_X = sqrt(sq(Voltage0_A1[d])+sq(Voltage0_A2[d]));
      Voltage0_theta = atan2(Voltage0_A2[d], Voltage0_A1[d]);
     }
    
    // Voltage 2
    Voltage1_B = Voltage1_sum/500;
    for(int e = 0; e < 500; e++){
      Voltage1_A1[e] = 2*Voltage1_B*sin(angle[e]);
      Voltage1_A2[e] = 2*Voltage1_B*sin(angle[e]);
      Voltage1_X = sqrt(sq(Voltage1_A1[e])+sq(Voltage1_A2[e]));
      Voltage1_theta = atan2(Voltage1_A2[e], Voltage1_A1[e]);
     }

    // Voltage 3
    Voltage2_B = Voltage2_sum/500;
    for(int f = 0; f < 500; f++){
      Voltage2_A1[f] = 2*Voltage2_B*sin(angle[f]);
      Voltage2_A2[f] = 2*Voltage2_B*sin(angle[f]);
      Voltage2_X = sqrt(sq(Voltage2_A1[f])+sq(Voltage2_A2[f]));
      Voltage2_theta = atan2(Voltage2_A2[f], Voltage2_A1[f]);
     }

    print_flag = true;
  }

  else if (timer_count >= 8000) { // after 9.5sec  // 20
    for(int k = 0; k < 500; k++){

      Current_data_arr0[k] = 0;
      Current_data_arr1[k] = 0;
      Current_data_arr2[k] = 0;

      Voltage_data_arr0[k] = 0;
      Voltage_data_arr1[k] = 0;
      Voltage_data_arr2[k] = 0;

      Current0_A1[k] = 0;
      Current0_A2[k] = 0; 
      Current1_A1[k] = 0; 
      Current1_A2[k] = 0; 
      Current2_A1[k] = 0; 
      Current2_A2[k] = 0;
      Voltage0_A1[k] = 0; 
      Voltage0_A2[k] = 0; 
      Voltage1_A1[k] = 0; 
      Voltage1_A2[k] = 0; 
      Voltage2_A1[k] = 0; 
      Voltage2_A2[k] = 0;
    }

    Current0_sum = 0, Current0_B = 0, Current0_X = 0, Current0_theta = 0;
    Current1_sum = 0, Current1_B = 0, Current1_X = 0, Current1_theta = 0;
    Current2_sum = 0, Current2_B = 0, Current2_X = 0, Current2_theta = 0;

    Voltage0_sum = 0, Voltage0_B = 0, Voltage0_X = 0, Voltage0_theta = 0;
    Voltage1_sum = 0, Voltage1_B = 0, Voltage1_X = 0, Voltage1_theta = 0;
    Voltage2_sum = 0, Voltage2_B = 0, Voltage2_X = 0, Voltage2_theta = 0;


    timer_count = 0;
    i = 0;
  }

  else if (timer_count == 7000) { // 19
    if (print_flag == true) {
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
      Serial.print(Current0_sum);
      Serial.print('\t');
      Serial.print(Current1_sum);
      Serial.print('\t');
      Serial.println(Current2_sum);
      print_flag = false;
    }
  }
}
