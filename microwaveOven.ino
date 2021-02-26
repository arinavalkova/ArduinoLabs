const int buttonPin = 12;
const int cookingLedPin = 8;
const int switchPin = 7;
const int backlightLedPin = 13;

const int sec5 = 5000;

int buttonState = 0;
int cookingLedState = 0;
int backlightLedState = 0;
int switchState = 0;
int timer = 0;
int timeCount = 0;

void setup() {
  pinMode(cookingLedPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(backlightLedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (timer != 0 && millis() - timer >= timeCount) {
    timer = 0;
    cookingLedState = false;
    backlightLedState = false;
  }
  
  if (!buttonState) {
    if (digitalRead(buttonPin) && !switchState) {
      //проверка на то что микроволновка закрыта
      if (timer == 0) {
        timer = millis();
        timeCount = sec5;
      } else {
        timeCount += sec5;
      }
      backlightLedState = true;
      cookingLedState = true;
      buttonState = true;
    }
  } else if (buttonState){
    if (!digitalRead(buttonPin)) {
      buttonState = false;
    }
  }
  
  if (!switchState) {
    if (digitalRead(switchPin) && timer == 0) {
      //проверка на то что микроволновка не включена сейчас
      backlightLedState = true;
      switchState = true;
    }
  } else if (switchState){
    if (!digitalRead(switchPin)) {
      backlightLedState = false;
      switchState = false;
    }
  }
  
  if (timer != 0) {
    int currentTime = timeCount - (millis() - timer);
    Serial.println(currentTime);
  }
  digitalWrite(cookingLedPin, cookingLedState);
  digitalWrite(backlightLedPin, backlightLedState);
} 
