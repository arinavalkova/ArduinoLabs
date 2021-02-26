#define LETTERS_COUNT 26
#define BITS_SIZE 8
#define MAX_INPUT_BUF_SIZE 8

char morseCodes[LETTERS_COUNT] = {
                   (1 << 1) | (1 << 6)
                 , (1 << 7) | (1 << 0)
                 , (1 << 6) | (1 << 5) | (1 << 2) | (1 << 1)
                 , (1 << 6) | (1 << 5) | (1 << 1) | (1 << 0)
                 , (1 << 6) | (1 << 5) | (1 << 0)
                 , (1 << 5)
                 , (1 << 7) | (1 << 3)
                 , (1 << 7) | (1 << 1) | (1 << 0)
                 , (1 << 6)
                 , (1 << 7) | (1 << 3) | (1 << 2) | (1 << 1)
                 , (1 << 6) | (1 << 5) | (1 << 2) | (1 << 0)
                 , (1 << 7) | (1 << 1)
                 , (1 << 6) | (1 << 1) | (1 << 0)
                 , (1 << 6) | (1 << 0)
                 , (1 << 6) | (1 << 5) | (1 << 2) | (1 << 1) | (1 << 0)
                 , (1 << 7) | (1 << 2) | (1 << 1)
                 , (1 << 6) | (1 << 5) | (1 << 1)
                 , (1 << 6) | (1 << 5)
                 , (1 << 5) | (1 << 0)
                 , (1 << 6) | (1 << 5) | (1 << 2)
                 , (1 << 7) | (1 << 2)
                 , (1 << 7)
                 , (1 << 7) | (1 << 2) | (1 << 0)
                 , (1 << 7) | (1 << 3) | (1 << 1) | (1 << 0)
                 , (1 << 7) | (1 << 3) | (1 << 2) | (1 << 0)
                 , (1 << 7) | (1 << 3) | (1 << 0)
};

char letters[LETTERS_COUNT] = {
                      'a', 'b', 'w', 'g', 'd', 'e', 'v', 'z', 'i', 'j', 'k', 'l', 'm'
                    , 'n', 'o', 'p', 'r', 's', 't', 'u', 'f', 'h', 'c', 'q', 'y', 'x'
};

char findCodeByLetter(char letter) {
    for (short i = 0; i < LETTERS_COUNT; i++) {
        if (letter == letters[i]) {
            return morseCodes[i];
        }
    }
    return 0;
}

int getCountOfMorseDigits(char morseCode) {
     char first = (morseCode & (1 << 7)) ? 1 : 0
            ,second = (morseCode & (1 << 6)) ? 1 : 0
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

char input[MAX_INPUT_BUF_SIZE];
short currentInputPos = 0;
short currentReadPos = 0;
short currentBufSize = 0;

char currentProcessedLetter;
char currentProcessedLetterFlag = 0;
char firstLoop = 1;

void setup() {
  Serial.begin(9600);
}

void loop() {
//    for (int j = 0; j < BITS_SIZE; j++) {
//        if (codes[i] & 0x80) {
//            printf("1");
//        } else {
//            printf("0");
//        }
//        codes[i] = codes[i] << 1;
//    }
  if (currentReadPos < currentInputPos && currentProcessedLetterFlag == 0) {
    currentProcessedLetter = input[currentReadPos];
    currentProcessedLetter = findCodeByLetter(currentProcessedLetter);
    currentReadPos++;
    if (currentReadPos == MAX_INPUT_BUF_SIZE) {
      currentReadPos = 0;
    }
    currentProcessedLetterFlag = 1;
    currentBufSize--;
  }
  
  if (currentProcessedLetterFlag != 0) {
     for (int j = 0; j < BITS_SIZE; j++) {
        if (currentProcessedLetter & 0x80) {
            Serial.print("1");
        } else {
            Serial.print("0");
        }
        currentProcessedLetter = currentProcessedLetter << 1;
    }
    Serial.println();
    currentProcessedLetterFlag = 0;
    //когда закончим работу с символом то сменить флаг
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
  
  //for (char i = 0; i < currentInputPos; i++) {
    //Serial.print(input[i]);
    //if (i + 1 == currentInputPos) {
      //Serial.println();
    //}
  //}
}
