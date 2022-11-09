// joystick pins
const int pinX = A0;
const int pinY = A1;
const int pinSW = 2;

int xValue;
int yValue;
int swValue;

byte currentSwValue = HIGH;
byte lastSwValue = HIGH;

// segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

const int segSize = 8; // number of pins

// map pins from 4-11 to 0-7
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

// actual segments states
byte segmentsStates[segSize] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};

int currentSegment = 7;
byte currentSegmentState = LOW;

// last time when a segment blinked and duration of the blinks
unsigned long previousBlink = 0;
const long intervalBlink = 300;

// debounce values
unsigned long lastDebounceTimeSw = 0;
unsigned long debounceDelaySw = 100;
unsigned long lastDebounceTimeJoy = 0;
unsigned long debounceDelayJoy = 200;

// thresholds for joystick
const int minThresholdCenter = 300;
const int maxThresholdCenter = 800;
const int minThresholdMargin = 70;
const int maxThresholdMargin = 1000;

// threshold for short button press and long button press
unsigned long shortOrLong = 300;

// directions
const int noOfDirections = 4;
int up = 0;
int down = 1;
int right = 2;
int left = 3;
int move;

bool joyMoved = true;
bool lastJoyMoved = false;
bool currentJoyMove = false;

// the movements available for each segment depending on where it is located
byte segmentsMovements[segSize][noOfDirections] = {
  //Up Dn LT RT
  { 0, 6, 5, 1 },  // a  0
  { 0, 6, 5, 1 },  // b  1
  { 6, 3, 4, 7 },  // c  2
  { 6, 3, 4, 2 },  // d  3
  { 6, 3, 4, 2 },  // e  4
  { 0, 6, 5, 1 },  // f  5
  { 0, 3, 6, 6 },  // g  6
  { 7, 7, 2, 7 }   // dp 7
};

// the current state
int state = 1;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }

  pinMode(pinSW, INPUT_PULLUP);
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swValue = digitalRead(pinSW);
/*
  Serial.print("xValue: ");
  Serial.print(xValue);
  Serial.print(", yValue: ");
  Serial.print(yValue);
  Serial.print(", swValue: ");
  Serial.println(swValue);
*/
  setBackSegStatus();

  if (state == 1) {
    blink();
    joyMoveState1();
  } else {
    joyMoveState2();
  }

  swPress();
}

/* joy movement for state 1: Checks if the joystick has not moved by fitting the
values between two central thresholds. It considers the joystick to have moved 
only if its values reach the upper threshold and if it exceeded the bounce. */
void joyMoveState1() {
  if ((minThresholdCenter <= xValue && xValue <= maxThresholdCenter) and (minThresholdCenter <= yValue && yValue <= maxThresholdCenter)) {
    joyMoved = false;
  }

  if (xValue < minThresholdMargin && joyMoved == false) {
    move = left;
    joyMoved = true;
  }

  if (xValue > maxThresholdMargin && joyMoved == false) {
    move = right;
    joyMoved = true;
  }

  if (yValue < minThresholdMargin && joyMoved == false) {
    move = down;
    joyMoved = true;
  }

  if (yValue > maxThresholdMargin && joyMoved == false) {
    move = up;
    joyMoved = true;
  }

  if (lastJoyMoved != joyMoved) {
    lastDebounceTimeJoy = millis();
  }

  if (millis() - lastDebounceTimeJoy > debounceDelayJoy) {
    if (joyMoved != currentJoyMove) {
      currentJoyMove = joyMoved;
      
      if (currentJoyMove == true) {
        currentSegment = segmentsMovements[currentSegment][move];
      }
    }
  }

  lastJoyMoved = joyMoved;
}

/* joy movement for state 2: If joystick a exceeds the upper threshold then it
 turns ON or OFF the current segment. */
void joyMoveState2() {
  if (xValue < minThresholdCenter) {
    segmentsStates[currentSegment] = LOW;
  }

  if (xValue > maxThresholdCenter) {
    segmentsStates[currentSegment] = HIGH;
  }

  digitalWrite(segments[currentSegment], segmentsStates[currentSegment]);
}

/* blink the current segment for state 1 */
void blink() {
  if (millis() - previousBlink >= intervalBlink) {
    previousBlink = millis();
    digitalWrite(segments[currentSegment], currentSegmentState);
    currentSegmentState = !currentSegmentState;
  }
}

/* set back new and old values of all segments */
void setBackSegStatus() {
  for (int i = 0; i < segSize; ++i) {
    if (i == currentSegment) {
      continue;
    }

    digitalWrite(segments[i], segmentsStates[i]);
  }
}

/* for the short press of the button goes from one state to another (from
state 1 to state 2 and vice versa). For long press of the button in state 1,
reset the segments by turning them off and set the current segment as the
Decimal Point. */
void swPress() {
  if (swValue == LOW and lastSwValue == HIGH) {
    lastDebounceTimeSw = millis();
  }

  if (millis() - lastDebounceTimeSw > debounceDelaySw) {
    if (swValue != currentSwValue) {
      currentSwValue = swValue;

      if (currentSwValue == HIGH) {
        unsigned long pressDuration = millis() - lastDebounceTimeSw;

        if (pressDuration < shortOrLong) {
          if (state == 1) {
            state = 2;
          } else if (state == 2) {
            state = 1;
          }
        } else {
          if (state == 1) {
            resetSegLOW();
          }
        }
      }
    }
  }
  
  lastSwValue = swValue;
}

/* clears all segments and sets the Decimal Point as the current segment */
void resetSegLOW() {
  for (int i = 0; i < segSize; i++) {
    segmentsStates[i] = LOW;
  }

  currentSegment = 7;
}