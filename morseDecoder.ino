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

typedef struct tree {
    char symbol;
    struct tree* left; //dot
    struct tree* right; //dash
} Tree;

volatile Tree* tree;
volatile Tree* currentNode;

Tree* createNode(char symbol, Tree* left, Tree* right) {
    Tree* newNode = (Tree*) malloc (sizeof(Tree));
    newNode->symbol = symbol;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

void createMorseTree() {
    Tree* end = createNode('*', NULL, NULL);
    Tree* h = createNode('h', NULL, NULL);
    Tree* v = createNode('v', NULL, NULL);
    Tree* f = createNode('f', NULL, end);
    Tree* l = createNode('l', NULL, NULL);
    Tree* p = createNode('p', NULL, NULL);
    Tree* j = createNode('j', NULL, NULL);
    Tree* b = createNode('b', NULL, NULL);
    Tree* x = createNode('x', NULL, NULL);
    Tree* c = createNode('c', NULL, NULL);
    Tree* y = createNode('y', NULL, NULL);
    Tree* z = createNode('z', NULL, NULL);
    Tree* q = createNode('q', NULL, NULL);
    Tree* s = createNode('s', h, v);
    Tree* u = createNode('u', f, NULL);
    Tree* r = createNode('r', l, NULL);
    Tree* w = createNode('w', p, j);
    Tree* d = createNode('d', b, x);
    Tree* k = createNode('k', c, y);
    Tree* g = createNode('g', z, q);
    Tree* o = createNode('o', NULL, NULL);
    Tree* i = createNode('i', s, u);
    Tree* a = createNode('a', r, w);
    Tree* n = createNode('n', d, k);
    Tree* m = createNode('m', g, o);
    Tree* e = createNode('e', i, a);
    Tree* t = createNode('t', n, m);
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

char getSymbol() {
    char symbol = currentNode->symbol;
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
      && currentTime - timePrev >= TIME_STATE - TIME_SCATTER
      && signalState == ON) {
    signalState = OFF;
    goLeft();
  } else if (currentTime - timePrev >= 3 * TIME_STATE - TIME_SCATTER
             && signalState == ON) { // >=3 on -
    signalState = OFF;
    goRight();
  } else if (currentTime - timePrev <= TIME_STATE + TIME_SCATTER
      		&& currentTime - timePrev >= TIME_STATE - TIME_SCATTER
      		&& signalState == OFF){ //in 1 off
    signalState = ON;
  } else if (currentTime - timePrev <= 3 * TIME_STATE + TIME_SCATTER
             && currentTime - timePrev >= 3 * TIME_STATE - TIME_SCATTER
             && signalState == OFF) { //in 3  off a
    signalState = ON;
    addToBuffer(getSymbol());
  } else if (currentTime - timePrev <= 7 * TIME_STATE + TIME_SCATTER
             && currentTime - timePrev >= 7 * TIME_STATE - TIME_SCATTER
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
  attachInterrupt(digitalPinToInterrupt(2), buttonHandler, CHANGE);
  Serial.begin(9600);
  createMorseTree();
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
