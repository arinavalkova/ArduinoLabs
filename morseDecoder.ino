#define BUFF_SIZE 128
#define TIME_STATE 1000
#define TIME_SCATTER 100

enum SignalState { //the button is pressed and released
    ON,
    OFF
};

volatile char buffer[BUFF_SIZE];
volatile char posAdd = 0, posRead = 0, countInBuff = 0;

volatile unsigned long timePrev = 0;
volatile SignalState signalState = OFF;

struct tree {
    char symbol;
    struct tree* left; //dot
    struct tree* right; //dash
};

volatile struct tree* tree;
volatile struct tree* currentNode;

struct tree* createNode(char symbol, struct tree* left, struct tree* right) {
    struct tree* newNode = (struct tree*) malloc (sizeof(struct tree));
    newNode->symbol = symbol;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

void createMorseTree() {
    struct tree* end = createNode('*', NULL, NULL);
    struct tree* h = createNode('h', NULL, NULL);
    struct tree* v = createNode('v', NULL, NULL);
    struct tree* f = createNode('f', NULL, end);
    struct tree* l = createNode('l', NULL, NULL);
    struct tree* p = createNode('p', NULL, NULL);
    struct tree* j = createNode('j', NULL, NULL);
    struct tree* b = createNode('b', NULL, NULL);
    struct tree* x = createNode('x', NULL, NULL);
    struct tree* c = createNode('c', NULL, NULL);
    struct tree* y = createNode('y', NULL, NULL);
    struct tree* z = createNode('z', NULL, NULL);
    struct tree* q = createNode('q', NULL, NULL);
    struct tree* s = createNode('s', h, v);
    struct tree* u = createNode('u', f, NULL);
    struct tree* r = createNode('r', l, NULL);
    struct tree* w = createNode('w', p, j);
    struct tree* d = createNode('d', b, x);
    struct tree* k = createNode('k', c, y);
    struct tree* g = createNode('g', z, q);
    struct tree* o = createNode('o', NULL, NULL);
    struct tree* i = createNode('i', s, u);
    struct tree* a = createNode('a', r, w);
    struct tree* n = createNode('n', d, k);
    struct tree* m = createNode('m', g, o);
    struct tree* e = createNode('e', i, a);
    struct tree* t = createNode('t', n, m);
    tree = createNode(' ', e, t);
    currentNode = tree;
}

void goLeft() {
    if (currentNode != NULL) {
        currentNode = currentNode->left;
    }
}

void goRight() {
    if (currentNode != NULL) {
        currentNode = currentNode->right;
        if (currentNode->symbol == '*') {
            currentNode = tree;
        }
    }
}

//нужно подождать какое то время вначале

char getSymbol() {
    char symbol;
    if (currentNode != NULL) {
        symbol = currentNode->symbol;
    } else {
        symbol = '*';
    }
    currentNode = tree;
    return symbol;
}

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
        && currentTime - timePrev >= 0
        && signalState == ON) {
        signalState = OFF;
        goLeft();
    } else if (currentTime - timePrev >= TIME_STATE + TIME_SCATTER
               && signalState == ON) { // >=3 on -
        signalState = OFF;
        goRight();
    } else if (currentTime - timePrev <= TIME_STATE + TIME_SCATTER
               && currentTime - timePrev >= 0
               && signalState == OFF){ //in 1 off
        signalState = ON;
    } else if (currentTime - timePrev <= 3 * TIME_STATE + TIME_SCATTER
               && currentTime - timePrev >= TIME_STATE + TIME_SCATTER
               && signalState == OFF) { //in 3  off a
        signalState = ON;
        addToBuffer(getSymbol());
    } else if (currentTime - timePrev <= 7 * TIME_STATE + TIME_SCATTER
               && currentTime - timePrev >= 3 * TIME_STATE + TIME_SCATTER
               && signalState == OFF) { // in 7 off probel
        signalState = ON;
        addToBuffer(getSymbol());
        addToBuffer(' ');
    } else {
        signalState = ON;
    }
    timePrev = currentTime;
}

void setup()
{
    createMorseTree();
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
            Serial.print(getFromBufferRes);
        }
    }
}
