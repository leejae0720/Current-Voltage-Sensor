//#define ANALOG_PIN_TIMER_INTERVAL 100 // 1ms, 샘플링 시간 설정
#define CHANNEL_Current_1 36
#define CHANNEL_Current_2 39
#define CHANNEL_Current_3 34
#define CHANNEL_Voltage_1 35
#define CHANNEL_Voltage_2 32
#define CHANNEL_Voltage_3 33

volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

static long analogPinTimer = 0;
unsigned long thisMillis_old;
unsigned long pastMillis;
unsigned long deltaMillis = 0;
unsigned int timer_count = 0;
int Current_Value_1, Current_Value_2, Current_Value_3;
int Voltage_Value_1, Voltage_Value_2, Voltage_Value_3;

unsigned int switchflag = 0;  // 초기 상태
unsigned int timerflag = 0;

int i, j, k = 0; // 데이터 구별 숫자
int a, b = 0; // 데이터 계산에 사용 변수
int Current_Data_arr1[2000], Current_Data_arr2[2000], Current_Data_arr3[2000];
int Voltage_Data_arr1[2000], Voltage_Data_arr2[2000], Voltage_Data_arr3[2000];
int Total_Current_Data_arr1;
int Total_Current_Data_arr2;

void IRAM_ATTR onTimer() { // interrupt function
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;

  Current_Value_1 = analogRead(CHANNEL_Current_1);
  Current_Value_2 = analogRead(CHANNEL_Current_2);
  Current_Value_3 = analogRead(CHANNEL_Current_3);
  
  Current_Data_arr1[i] = Current_Value_1;
  Current_Data_arr2[j] = Current_Value_2;
  Current_Data_arr3[k] = Current_Value_3;

  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  timer = timerBegin(0, 80, true);  // run for 1msec second
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true); // 1msec timer
  timerAlarmEnable(timer);
}

void loop() {
  unsigned long thisMillis = millis();
  unsigned long deltaMillis = 0;

  if(interruptCounter > 0){ //interrupt function, 1msec 마다 동작
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;

    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }

  if(thisMillis - pastMillis >= 500){
    pastMillis = thisMillis;
    timerflag = 1;
  }
  /*
  if(timerflag == 1){
    timer_count += 1;

    if(timer_count == 20){
      switchflag = 1;      
    }
    else if(timer_count == 24){
      switchflag = 0;
      timer_count = 0;
      i = 0;
      j = 0;
    }
    timerflag = 0;
  }
  */
  if(switchflag == 0){
    
    //Voltage_Value_1 = analogRead(CHANNEL_Voltage_1);
    //Voltage_Value_2 = analogRead(CHANNEL_Voltage_2);
    //Voltage_Value_3 = analogRead(CHANNEL_Voltage_3);

    //Serial.print(thisMillis/1000.0);  // Sampleing Time 1ms setting
    //Serial.print(",");
    //Serial.println(Current_Value_1);
    //Serial.print(",");
    Serial.println(Current_Value_2);
    //Serial.print(",");
    //Serial.println(Current_Value_3);
    //Serial.print(Current_Data_arr1[i]);
    //Serial.print(",");
    //Serial.print(i);
    //Serial.print(",");
    //Serial.print(Current_Data_arr2[j]);
    //Serial.print(",");
    //Serial.println(j);
      
    i++;
    j++;

    if((thisMillis%300000) == 0){ // Operations per 5 minutes
      Serial.print("5555555555555555555555555555555555555");
    }
  }

  else if(switchflag == 1){
    
    for(a=0; a<=2000; a++){
      Total_Current_Data_arr1 += Current_Data_arr1[a];
    }

    for(b=0; b<2000; b++){
      Total_Current_Data_arr1 += Current_Data_arr1[b];
    }

    if(thisMillis != thisMillis_old) {
    deltaMillis = thisMillis-thisMillis_old;
    thisMillis_old = thisMillis;
    }
    analogPinTimer -= deltaMillis;

      if(analogPinTimer <= 0){
        //analogPinTimer += ANALOG_PIN_TIMER_INTERVAL;
        Serial.println("Calculation Mode");
            
      }    
    } 
  }