#define LETTERS_COUNT 26
#define MAX_INPUT_BUF_SIZE 128
#define TIME_STATE 500

char morseCodes[LETTERS_COUNT] = { //первые три бита - кол-во символов в морзе коде, остальные пять морзе код, где "1" 
        //- это тире, "0" - это точка.
        (1 << 1) | (1 << 6), (1 << 7) | (1 << 0), (1 << 7) | (1 << 2) | (1 << 0), (1 << 6) | (1 << 5) | (1 << 0),
        (1 << 5), (1 << 7) | (1 << 2), (1 << 6) | (1 << 5) | (1 << 1) | (1 << 0), (1 << 7), (1 << 6),
        (1 << 7) | (1 << 3) | (1 << 2) | (1 << 1), (1 << 6) | (1 << 5) | (1 << 2) | (1 << 0), (1 << 7) | (1 << 1),
        (1 << 6) | (1 << 1) | (1 << 0), (1 << 6) | (1 << 0), (1 << 6) | (1 << 5) | (1 << 2) | (1 << 1) | (1 << 0),
        (1 << 7) | (1 << 2) | (1 << 1), (1 << 7) | (1 << 3) | (1 << 1) | (1 << 0), (1 << 6) | (1 << 5) | (1 << 1),
        (1 << 6) | (1 << 5), (1 << 5) | (1 << 0), (1 << 6) | (1 << 5) | (1 << 2), (1 << 7) | (1 << 3),
        (1 << 6) | (1 << 5) | (1 << 2) | (1 << 1), (1 << 7) | (1 << 3) | (1 << 0),
        (1 << 7) | (1 << 3) | (1 << 2) | (1 << 0), (1 << 7) | (1 << 1) | (1 << 0)
};

int getCountOfMorseDigits(char morseCode) {
    char first = (morseCode & (1 << 7)) ? 1 : 0
    , second = (morseCode & (1 << 6)) ? 1 : 0
    , third = (morseCode & (1 << 5)) ? 1 : 0;
    if (first == 0 && second == 0 && third == 0) {
        return 0;
    } else if (first == 0 && second == 0 && third == 1) {
        return 1;
    } else if (first == 0 && second == 1 && third == 0) {
        return 2;
    } else if (first == 0 && second == 1 && third == 1) {
        return 3;
    } else if (first == 1 && second == 0 && third == 0) {
        return 4;
    } else if (first == 1 && second == 0 && third == 1) {
        return 5;
    }
}

char getCurrentCodeDigit(char code, char i) {
    return (code & (1 << i)) ? 1 : 0;
}

char input[MAX_INPUT_BUF_SIZE];
short currentInputPos = 0, currentReadPos = 0, currentBufSize = 0;
char currentProcessedLetter, firstLoop = 1, currentCountCodeDigits, currentDigit;
char ledPin = 12;

void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    if (currentReadPos < currentInputPos) {
        currentProcessedLetter = input[currentReadPos];
        currentReadPos++;
        if (currentReadPos == MAX_INPUT_BUF_SIZE) {
            currentReadPos = 0;
        }
        if (currentProcessedLetter != ' ') {
            currentProcessedLetter = morseCodes[currentProcessedLetter - 'a'];
            currentCountCodeDigits = getCountOfMorseDigits(currentProcessedLetter);

            for (char i = 0; i < currentCountCodeDigits; i++) {
                currentDigit = getCurrentCodeDigit(currentProcessedLetter, i);
                if (currentDigit == 0) {
                    digitalWrite(ledPin, 1);
                    delay(TIME_STATE);
                    digitalWrite(ledPin, 0);
                    delay(TIME_STATE);
                } else if (currentDigit == 1) {
                    digitalWrite(ledPin, 1);
                    delay(TIME_STATE * 3);
                    digitalWrite(ledPin, 0);
                    delay(TIME_STATE);
                }
            }
            delay(TIME_STATE * 3);
        } else {
            delay(TIME_STATE * 7);
        }
        currentBufSize--;
    }
    if (currentInputPos == currentReadPos && currentBufSize != 0 && !firstLoop) {
        return;
    }
    if (currentInputPos == MAX_INPUT_BUF_SIZE && currentReadPos == 0) {
        return;
    } else if (currentInputPos == MAX_INPUT_BUF_SIZE) {
        currentInputPos = 0;
    }
    if (Serial.available() > 0) {
        input[currentInputPos] = Serial.read();
        currentInputPos++;
        currentBufSize++;
        firstLoop = 0;
    }
}
