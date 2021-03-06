#define BUFF_SIZE 128
#define TIME_STATE 300
#define TIME_SCATTER 300

enum SignalState { //зажата кнопка или отпущена
  ON,
  OFF
};

volatile char buffer[BUFF_SIZE];
volatile char posAdd = 0, posRead = 0, countInBuff = 0;

volatile unsigned long timePrev = 0;
volatile SignalState signalState = OFF;

void addToBuffer(char simbol) {
  cli();
  buffer[posAdd] = simbol;
  if (posAdd + 1 == BUFF_SIZE) {
    posAdd = 0;
  } else {
    posAdd++;
  }
  countInBuff++;
  sei();
}

char popFromBuffer() {
  cli();
  char simbol = buffer[posRead];
  if (posRead + 1 == BUFF_SIZE) {
    posRead = 0;
  } else {
    posRead++;
  }
  countInBuff--;
  sei();
  return simbol;
}

bool isBufferEmpty() {
  bool isBuffEmptyRes;
  char countInBuffRes;
  cli();
  countInBuffRes = countInBuff;
  sei();
  if (countInBuffRes == 0) {
    return true;
  } else {
    return false;
  }
}

void buttonHandler() {
  unsigned long currentTime = millis();
  if (currentTime - timePrev <= TIME_STATE + TIME_SCATTER //in 1 on .
      && currentTime - timePrev >= TIME_STATE - TIME_SCATTER
      && signalState == ON) {
    signalState = OFF;
    addToBuffer('.');
  } else if (currentTime - timePrev >= 3 * TIME_STATE - TIME_SCATTER
             && signalState == ON) { // >=3 on -
    signalState = OFF;
    addToBuffer('-');
  } else if (currentTime - timePrev <= TIME_STATE + TIME_SCATTER
      		&& currentTime - timePrev >= TIME_STATE - TIME_SCATTER
      		&& signalState == OFF){ //in 1 off
    signalState = ON;
  } else if (currentTime - timePrev <= 3 * TIME_STATE + TIME_SCATTER
             && currentTime - timePrev >= 3 * TIME_STATE - TIME_SCATTER
             && signalState == OFF) { //in 3  off a
    signalState = ON;
    //print simbol
    addToBuffer('a');
  } else if (currentTime - timePrev <= 7 * TIME_STATE + TIME_SCATTER
             && currentTime - timePrev >= 7 * TIME_STATE - TIME_SCATTER
             && signalState == OFF) { // in 7 off probel
    signalState = ON;
    //print simbol
    addToBuffer(' ');
  } else {
    signalState = ON;
  }
  timePrev = currentTime;
}

bool checkEndSimbolInTree() {
  return false;
  //TO DO
}

void setup()
{
  attachInterrupt(digitalPinToInterrupt(2), buttonHandler, CHANGE);
  Serial.begin(9600);
}

void loop()
{
  while (1) {
  	bool isBufferEmptyRes = isBufferEmpty();
    if (isBufferEmptyRes) {
      break;
    } else {
      char getFromBufferRes = popFromBuffer();
      Serial.println(getFromBufferRes);
    }
  }
}
