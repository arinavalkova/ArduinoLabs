#define BUFF_SIZE 128

volatile char buffer[BUFF_SIZE];
volatile char posAdd = 0, posRead = 0, countInBuff = 0;
volatile char bounceProtection = 0;
volatile unsigned long timePrev = 0;

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
  posRead++;
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
  if (millis() - timePrev >= 200) {
    bounceProtection = 0;
    timePrev = millis();
  }
  if (!bounceProtection) {
    addToBuffer('1');
    bounceProtection = 1;
  }
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
