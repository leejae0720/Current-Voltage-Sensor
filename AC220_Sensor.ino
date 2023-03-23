#define ANALOG_PIN_TIMER_INTERVAL 1000 // 1ms
#define CHANNEL_1 36
#define CHANNEL_2 39

int Channel_1_arr_size;

volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

static long analogPinTimer = 0;
unsigned long thisMillis_old;
int sensorValue_1, sensorValue_2;

unsigned long pastMillis_0 = 0; 
unsigned long pastMillis_1 = 0;
unsigned long pastMillis_2 = 0;
int timerflag = 0;  // 초기 상태

int i, j = 0;  // 데이터 구별 숫자
int Data_arr1[500];
int Data_arr2[500];

void IRAM_ATTR onTimer_1() { // interrupt function
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  timer = timerBegin(0, 80, true);  // run for 1 second
  timerAttachInterrupt(timer, &onTimer_1, true);
  timerAlarmWrite(timer, 1000000*60*5, true); // run for 5 minutes
  timerAlarmEnable(timer);

}

void loop() {
  
  unsigned long deltaMillis = 0;
  unsigned long thisMillis = millis();

  if(interruptCounter > 0){ //interrupt function, 5분 마다 동작
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;

    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }
  
  switch(timerflag){
    case 0: // timerflag = 0
      if (thisMillis != thisMillis_old) {
        deltaMillis = thisMillis-thisMillis_old;
        thisMillis_old = thisMillis;
      }
    
      analogPinTimer -= deltaMillis;

      if(analogPinTimer <= 0) {
        analogPinTimer += ANALOG_PIN_TIMER_INTERVAL;
        sensorValue_1 = analogRead(CHANNEL_1);
        sensorValue_2 = analogRead(CHANNEL_2);

        Data_arr1[i] = sensorValue_1;
        Data_arr2[j] = sensorValue_2;
            
        Serial.print(thisMillis/1000.0);  // Sampleing Time 1ms setting
        Serial.print(",");
        Serial.print(sensorValue_1);
        Serial.print(",");
        Serial.print(sensorValue_2);
        Serial.print(",");
        Serial.print(Data_arr1[i]);
        Serial.print(",");
        Serial.print(i);
        Serial.print(",");
        Serial.print(Data_arr2[j]);
        Serial.print(",");
        Serial.println(j);
      
        i++;
        j++;

        if((thisMillis%300000) == 0){ // Operations per 5 minutes
          Serial.print("5555555555555555555555555555555555555");
        } 
      }
    break;

    case 1: // timerflag = 1, 9.5 초간 수행 할 작업 실행
      //Serial.println("Data Calculation Start");
      if(thisMillis - pastMillis_1 > 9500){
        timerflag = 0;
        Serial.println("Set Position");
      }
      break;
 }

  if(thisMillis - pastMillis_0 >= 4000){  // 4초가 지났을때 timerflag = 1 로 변경
    pastMillis_0 = thisMillis;
    timerflag = 1;
    Serial.println("Data Calculation Start");
    
  }

  /*
  if(timerflag == 1){
    if(thisMillis - pastMillis_1 >= 9500){
      pastMillis_1 = thisMillis;
      Serial.println("Data Calculation End");
      digitalWrite(2, !(digitalRead(2)));
      delay(1000);
      timerflag = 0;
    }
  }
  /*
  if(thisMillis - pastMillis_1 >= 9500 && timerflag == 1){ // 9.5초가 지나고 timerflag = 1 일때, timerflag = 0 로 변경
    pastMillis_1 = thisMillis;
    digitalWrite(2, !(digitalRead(2)));
    delay(1000);
    timerflag = 0;
  }
  */
 
}
