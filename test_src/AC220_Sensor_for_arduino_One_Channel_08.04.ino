#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <math.h>

#define CHANNEL_Current_1 A0
#define CHANNEL_Voltage_1 A3


bool print_flag = false;
unsigned int serial_flag = 0;

// timer interrupt setting
unsigned int timer_count = 0;
int count = 0;
// data distinct digit
int i = 0;

unsigned int Current_data_arr0[500];
unsigned int Voltage_data_arr0[500];

// sin curve fitting value
float angle[500];
float fitting_sampling_time[500];

// sincurve fitting data variable
float Current0_theta = 0;
volatile float Current0_sum = 0, Current0_Odc = 0, Current0_X = 0;
float Current0_A1, Current0_A2, Current0_A1_sum, Current0_A2_sum;


float Voltage0_theta = 0;
volatile float Voltage0_sum = 0, Voltage0_Odc = 0, Voltage0_X = 0;
float Voltage0_A1, Voltage0_A2, Voltage0_A1_sum, Voltage0_A2_sum;

float Irms0, Vrms0, Pf0;
float Channel1_Power = 0;

void onTimer0() { // interrupt function
  if (timer_count >= 0 && timer_count < 500){  // 0 ~ 0.5 sec
    noInterrupts();
    Current_data_arr0[i] = analogRead(CHANNEL_Current_1);
    Voltage_data_arr0[i] = analogRead(CHANNEL_Voltage_1);

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
    fitting_sampling_time[t] = 0.001 * (t+1);
    angle[t] = 2 * PI * 60 * fitting_sampling_time[t];
  }
}

void loop() {
  if (timer_count == 1000) {  // 1 second, 
    
    // sin curve fitting function
    for (int j = 0; j < 500; j++){
      
      noInterrupts();
      Current0_A1_sum += Current_data_arr0[j] * sin(angle[j]);  // Current0_Odc
      Current0_A2_sum += Current_data_arr0[j] * cos(angle[j]);  // Current0_Odc
      Voltage0_A1_sum += Voltage_data_arr0[j] * sin(angle[j]);  // Voltage0_Odc
      Voltage0_A2_sum += Voltage_data_arr0[j] * cos(angle[j]);  // Voltage0_Odc
       
      interrupts();
    }

    // Sincurve fitting 
    // Current 1
    Current0_A1 = (2*Current0_A1_sum)/500;
    Current0_A2 = (2*Current0_A2_sum)/500;
    Current0_X = sqrt(sq(Current0_A1)+sq(Current0_A2));
    Current0_theta = atan2(Current0_A2 ,Current0_A1);

    
    // Voltage 1
    Voltage0_A1 = (2*Voltage0_A1_sum)/500;
    Voltage0_A2 = (2*Voltage0_A2_sum)/500;
    Voltage0_X = sqrt(sq(Voltage0_A1)+sq(Voltage0_A2));
    Voltage0_theta = atan2(Voltage0_A2, Voltage0_A1);

    print_flag = true;

  Irms0 = Current0_X / sqrt(2);
  Vrms0 = Voltage0_X / sqrt(2);

  Pf0 = cos((-Current0_theta) + Voltage0_theta);

  // Channel 1
  Channel1_Power = abs(Irms0 * Vrms0 * Pf0);
  }

  else if (timer_count >= 10000) { // after 9.5sec, 10sec reset
    for(int k = 0; k < 500; k++){

      Current_data_arr0[k] = 0;
      Voltage_data_arr0[k] = 0;
    }

    Current0_A1 = 0, Current0_A2 = 0, Voltage0_A1 = 0, Voltage0_A2 = 0;
    Current0_A1_sum = 0, Current0_A2_sum = 0; Voltage0_A1_sum = 0, Voltage0_A2_sum = 0;
    Current0_sum = 0, Current0_Odc = 0, Current0_X = 0, Current0_theta = 0;
    Voltage0_sum = 0, Voltage0_Odc = 0, Voltage0_X = 0, Voltage0_theta = 0;
    Pf0 = 0;
    Channel1_Power = 0;

    timer_count = 0;
    i = 0;
  }

  else if (timer_count == 9500) { // 9.5sec print
    if (print_flag == true) {
      Serial.print(Irms0);
      Serial.print("\t");
      Serial.print(Vrms0);
      Serial.print("\t");
      Serial.print(Pf0);
      Serial.print("\t");
      Serial.println(Channel1_Power);

      print_flag = false;
    }
  }
}
