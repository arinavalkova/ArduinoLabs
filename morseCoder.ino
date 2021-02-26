#include <stdio.h>
#include <io.h>
#define LETTERS_COUNT 26
#define BITS_SIZE 8
#define INPUT_BUF_SIZE 128

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

char findCodeForLetter(char letter) {
    for (int i = 0; i < LETTERS_COUNT; i++) {
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

char input[INPUT_BUF_SIZE];

int main() {
//    for (int j = 0; j < BITS_SIZE; j++) {
//        if (codes[i] & 0x80) {
//            printf("1");
//        } else {
//            printf("0");
//        }
//        codes[i] = codes[i] << 1;
//    }
    char letter;
    read(0, &letter, 1);
    printf("%d\n", getCountOfMorseDigits(findCodeForLetter(letter)));
    return 0;
}
