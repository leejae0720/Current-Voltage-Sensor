#define ANALOG_PIN_TIMER_INTERVAL 1 // 1ms
#define CHANNEL_1 36 

int Channel_1_arr_size;

volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

static long analogPinTimer = 0;
unsigned long thisMillis_old;
int sensorValue;

int swtich_number;  // '1': Data collection, '2': Data Calculation

unsigned long pastMillis = 0; 
int timerflag = 0; 

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

/*
  if(thisMillis - pastMillis >= 500){ // Doing 0.5sec
        pastMillis = thisMillis;
        timerflag = 1;
  }

  if(timerflag == 1){
        digitalWrite(2, !(digitalRead(2)));
        timerflag = 0;
  }
*/

  if(interruptCounter > 0){ //interrupt function
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;

    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }
  
 
  if (thisMillis != thisMillis_old) {

    deltaMillis = thisMillis-thisMillis_old;
    thisMillis_old = thisMillis;
  }
 
  analogPinTimer -= deltaMillis;

  if(analogPinTimer <= 0) {
    analogPinTimer += ANALOG_PIN_TIMER_INTERVAL;
    sensorValue = analogRead(CHANNEL_1);

    Serial.print(thisMillis/1000.0);
    Serial.print(",");
    Serial.println(sensorValue);
    timerflag = 1;

  if(timerflag = 1){
    digitalWrite(2, !(digitalRead(2)));
    timerflag = 0;
  }
    if((thisMillis%300000) == 0){ // Operations per 5 minutes
      Serial.print("5555555555555555555555555555555555555");
    } 
  }

  if(thisMillis - pastMillis >= 2000){ // Doing 2 second
  pastMillis = thisMillis;
  Serial.println("on");
  timerflag = 1;
  }

  if(timerflag = 1){ // 2초가 지났을때 수행할 작업 함수 작성
    digitalWrite(2, !(digitalRead(2)));
    timerflag = 0;
  }
} 