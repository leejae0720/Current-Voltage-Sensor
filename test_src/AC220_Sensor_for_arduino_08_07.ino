#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <math.h>

#define CHANNEL_Current_0 A0
#define CHANNEL_Current_1 A1
#define CHANEEL_Current_2 A2
#define CHANNEL_Voltage_0 A3
#define CHANNEL_Voltage_1 A4
#define CHANNEL_Voltage_2 A5

#define Current_sensor_Value 10

bool print_flag = false;
unsigned int serial_flag = 0;

// timer interrupt setting
unsigned int timer_count = 0;
int count = 0;
// data distinct digit
int i = 0;

unsigned int Current_data_arr0[500] = {0};
unsigned int Current_data_arr1[500] = {0};
unsigned int Current_data_arr2[500] = {0};
unsigned int Voltage_data_arr0[500] = {0};
unsigned int Voltage_data_arr1[500] = {0};
unsigned int Voltage_data_arr2[500] = {0};
int angle[500] = {0};
float fitting_sampling_time[500] = {0};

float Pf0, Irms0_out, Vrms0_out;
float Channel1_Power = 0;

float Pf1, Irms1_out, Vrms1_out;
float Channel2_Power = 0;

float Pf2, Irms2_out, Vrms2_out;
float Channel3_Power = 0;

// interrupt function
void onTimer0() { // interrupt function
  if (timer_count >= 0 && timer_count < 500){  // 0 ~ 0.5 sec
    noInterrupts();
    Current_data_arr0[i] = analogRead(CHANNEL_Current_0);
    Voltage_data_arr0[i] = analogRead(CHANNEL_Voltage_0);
    Current_data_arr1[i] = analogRead(CHANNEL_Current_1);
    Voltage_data_arr1[i] = analogRead(CHANNEL_Voltage_1);
    Current_data_arr2[i] = analogRead(CHANEEL_Current_2);
    Voltage_data_arr2[i] = analogRead(CHANNEL_Voltage_2);

    i++;
    interrupts();
  }
  timer_count ++; 
}

// Caluation function
void cal(){
  
  float Current0_A1_sum = 0, Current0_A2_sum = 0;
  float Voltage0_A1_sum = 0, Voltage0_A2_sum = 0;
  float Current1_A1_sum = 0, Current1_A2_sum = 0;
  float Voltage1_A1_sum = 0, Voltage1_A2_sum = 0;
  float Current2_A1_sum = 0, Current2_A2_sum = 0;
  float Voltage2_A1_sum = 0, Voltage2_A2_sum = 0;

  // sin curve fitting
  for (int j = 0; j < 500; j++){
      
    noInterrupts();
    Current0_A1_sum += Current_data_arr0[j] * sin(angle[j]);  // Current0_Odc
    Current0_A2_sum += Current_data_arr0[j] * cos(angle[j]);  // Current0_Odc
    Voltage0_A1_sum += Voltage_data_arr0[j] * sin(angle[j]);  // Voltage0_Odc
    Voltage0_A2_sum += Voltage_data_arr0[j] * cos(angle[j]);  // Voltage0_Odc

    Current1_A1_sum += Current_data_arr1[j] * sin(angle[j]);  // Current1_Odc
    Current1_A2_sum += Current_data_arr1[j] * cos(angle[j]);  // Current1_Odc
    Voltage1_A1_sum += Voltage_data_arr1[j] * sin(angle[j]);  // Voltage1_Odc
    Voltage1_A2_sum += Voltage_data_arr1[j] * cos(angle[j]);  // Voltage1_Odc

    Current2_A1_sum += Current_data_arr2[j] * sin(angle[j]);  // Current2_Odc
    Current2_A2_sum += Current_data_arr2[j] * cos(angle[j]);  // Current2_Odc
    Voltage2_A1_sum += Voltage_data_arr2[j] * sin(angle[j]);  // Voltage2_Odc
    Voltage2_A2_sum += Voltage_data_arr2[j] * cos(angle[j]);  // Voltage2_Odc
    interrupts();
  }

  // Sincurve fitting 
  // Current 1
  float Current0_A1 = (2*Current0_A1_sum)/500;
  float Current0_A2 = (2*Current0_A2_sum)/500;
  float Current0_X = sqrt(sq(Current0_A1)+sq(Current0_A2));
  float Current0_theta = atan2(Current0_A2 ,Current0_A1);

  // Voltage 1
  float Voltage0_A1 = (2*Voltage0_A1_sum)/500;
  float Voltage0_A2 = (2*Voltage0_A2_sum)/500;
  float Voltage0_X = sqrt(sq(Voltage0_A1)+sq(Voltage0_A2));
  float Voltage0_theta = atan2(Voltage0_A2, Voltage0_A1);

  // Current 2
  float Current1_A1 = (2*Current1_A1_sum)/500;
  float Current1_A2 = (2*Current1_A2_sum)/500;
  float Current1_X = sqrt(sq(Current1_A1)+sq(Current1_A2));
  float Current1_theta = atan2(Current1_A2 ,Current1_A1);

  // Voltage 2
  float Voltage1_A1 = (2*Voltage1_A1_sum)/500;
  float Voltage1_A2 = (2*Voltage1_A2_sum)/500;
  float Voltage1_X = sqrt(sq(Voltage1_A1)+sq(Voltage1_A2));
  float Voltage1_theta = atan2(Voltage1_A2, Voltage1_A1);

  // Current 3
  float Current2_A1 = (2*Current2_A1_sum)/500;
  float Current2_A2 = (2*Current2_A2_sum)/500;
  float Current2_X = sqrt(sq(Current2_A1)+sq(Current2_A2));
  float Current2_theta = atan2(Current2_A2 ,Current2_A1);

  // Voltage 3
  float Voltage2_A1 = (2*Voltage2_A1_sum)/500;
  float Voltage2_A2 = (2*Voltage2_A2_sum)/500;
  float Voltage2_X = sqrt(sq(Voltage2_A1)+sq(Voltage2_A2));
  float Voltage2_theta = atan2(Voltage2_A2, Voltage2_A1);

  float Irms0 = Current0_X / sqrt(2);
  float Vrms0 = Voltage0_X / sqrt(2);

  float Irms1 = Current1_X / sqrt(2);
  float Vrms1 = Voltage1_X / sqrt(2);

  float Irms2 = Current2_X / sqrt(2);
  float Vrms2 = Voltage2_X / sqrt(2);

  Irms0_out = (Irms0 * 5 * Current_sensor_Value) / 1024;
  Vrms0_out = (Vrms0 * 5 * 200) / 1024;

  Irms1_out = (Irms1 * 5 * Current_sensor_Value) / 1024;
  Vrms1_out = (Vrms1 * 5 * 200) / 1024;

  Irms2_out = (Irms2 * 5 * Current_sensor_Value) / 1024;
  Vrms2_out = (Vrms2 * 5 * 200) / 1024;

  Pf0 = cos((-Current0_theta) + Voltage0_theta);
  Pf1 = cos((-Current1_theta) + Voltage1_theta);
  Pf2 = cos((-Current2_theta) + Voltage2_theta);

  // Channel 1
  Channel1_Power = abs(Irms0_out * Vrms0_out * Pf0);
  Channel2_Power = abs(Irms1_out * Vrms1_out * Pf1);
  Channel3_Power = abs(Irms2_out * Vrms2_out * Pf2);
  
  print_flag = true;
}

void Serial_Send_function(){
  Serial.print(Irms0_out); Serial.print("\t");
  Serial.print(Vrms0_out); Serial.print("\t");
  Serial.print(Pf0); Serial.print("\t");
  Serial.print(Channel1_Power); Serial.print("\t");
  Serial.print(Irms1_out); Serial.print("\t");
  Serial.print(Vrms1_out); Serial.print("\t");
  Serial.print(Pf1); Serial.print("\t");
  Serial.print(Channel2_Power); Serial.print("\t");
  Serial.print(Irms2_out); Serial.print("\t");
  Serial.print(Vrms2_out); Serial.print("\t");
  Serial.print(Pf2); Serial.print("\t");
  Serial.println(Channel3_Power);

  print_flag = false;
}

void setup() {
  Serial.begin(115200);

  MsTimer2::set(1, onTimer0); // 0.001 sec Timer
  MsTimer2::start();
  for(int t = 0; t < 500; t++){
    fitting_sampling_time[t] = 0.001 * (t+1);
    angle[t] = 2 * PI * 60 * fitting_sampling_time[t];
  }
}

void loop() {
  if (timer_count == 1000) {  // 1 second, 
   cal(); 
  }

  else if (timer_count >= 10000) { // after 9.5sec, 10sec reset
    for(int k = 0; k < 500; k++){

      Current_data_arr0[k] = 0;
      Voltage_data_arr0[k] = 0;
      Current_data_arr1[k] = 0;
      Voltage_data_arr1[k] = 0;
      Current_data_arr2[k] = 0;
      Voltage_data_arr2[k] = 0;
    }
    
    Irms0_out = 0, Vrms0_out = 0, Pf0 = 0, Channel1_Power = 0;
    Irms1_out = 0, Vrms1_out = 0, Pf1 = 0, Channel2_Power = 0;
    Irms2_out = 0, Vrms2_out = 0, Pf2 = 0, Channel3_Power = 0;

    timer_count = 0;
    i = 0;
  }

  else if (timer_count == 9500) { // 9.5sec print
    if (print_flag == true) {
      Serial_Send_function();
    }
  }
}