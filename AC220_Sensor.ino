#define ANALOG_PIN_TIMER_INTERVAL 1000 // 1ms
#define CHANNEL_1 36
#define CHANNEL_2 39

volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

static long analogPinTimer = 0;
unsigned long thisMillis_old;
unsigned long pastMillis; 
unsigned int timer_count = 0;
int sensorValue_1, sensorValue_2;

unsigned int switchflag = 0;  // 초기 상태
unsigned int timerflag = 0;

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
  
  if(switchflag == 0){
    if(thisMillis != thisMillis_old) {
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
  }

  else if(switchflag == 1){
    //Serial.println("Calculation Mode");
  }
  
  if(thisMillis - pastMillis >= 500){
    pastMillis = thisMillis;
    timerflag = 1;
  }
  
  if(timerflag == 1){
    timer_count += 1;

    if(timer_count == 8){
      switchflag = 1;      
    }
    else if(timer_count == 24){
      switchflag = 0;
      timer_count = 0;
    }

    timerflag = 0;
  }
}

/*
  if(thisMillis - pastMillis_0 >= 4000){  // 4초가 지났을때 switchflag = 1 로 변경
    if(switchflag == 0){
      pastMillis_0 = thisMillis;
      pastMillis_1 = thisMillis;
      switchflag = 1;
      Serial.println("Data Calculation Start");
    }
    else if(switchflag == 1){
      pastMillis_0 = thisMillis;
      
    }
  }

  if(thisMillis - pastMillis_1 >= 10000){  // 10초 간 작업 수행
    if(switchflag == 1){
      pastMillis_0 = thisMillis;
      pastMillis_1 = thisMillis;
      switchflag = 0;
      Serial.println("Set Postition");
    }
    else if(switchflag == 0){
      pastMillis_1 = thisMillis;
    }
  }
}

  if(thisMillis - pastMillis_1 >= 13500){ // 4초 후 9.5초간 내용 입력
    pastMillis_1 = thisMillis;
    switchflag = 0;
    Serial.println("Set Position");
  }
  
  if(switchflag == 1){
    if(thisMillis - pastMillis_1 >= 9500){
      pastMillis_1 = thisMillis;
      Serial.println("Data Calculation End");
      digitalWrite(2, !(digitalRead(2)));
      delay(1000);
      switchflag = 0;
    }
  }

  if(thisMillis - pastMillis_1 >= 9500 && switchflag == 1){ // 9.5초가 지나고 switchflag = 1 일때, switchflag = 0 로 변경
    pastMillis_1 = thisMillis;
    digitalWrite(2, !(digitalRead(2)));
    delay(1000);
    switchflag = 0;
  }
  */
 

