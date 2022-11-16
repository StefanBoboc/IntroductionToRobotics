// DS = [D]ata [S]torage - data
// STCP = [ST]orage [C]lock [P]in - latch
// SHCP = [SH]ift register [C]lock [P]in - clock

const int dataPin = 12;   // DS to 12 on Shift Register
const int clockPin = 11;  // SHCP to 11 on Shift Register
const int latchPin = 10;  // STCP to 10 on Shift Register

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int pinX = A0;
const int pinY = A1;
const int pinSW = 2;

int xValue;
int yValue;
int swValue;

int currentDisplay = 0;

byte currentSwValue = HIGH;
byte lastSwValue = HIGH;

bool currentJoyMove = false;
bool lastJoyMoved = false;
bool joyMoved = true;

byte axis;
byte xAxis = 0;
byte yAxis = 1;

int direction;
byte onOffBlink = 1;

// last time when a Decimal Point blinked and duration of the blinks
unsigned long previousBlink = 0;
const long intervalBlink = 500;

// debounce values
unsigned long lastDebounceTimeSw = 0;
unsigned long debounceDelaySw = 100;
unsigned long lastDebounceTimeJoy = 0;
unsigned long debounceDelayJoy = 200;

// thresholds for joystick
const int minThresholdCenter = 300;
const int maxThresholdCenter = 800;
const int minThresholdMargin = 70;
const int maxThresholdMargin = 900;

// threshold for short button press and long button press
unsigned long shortOrLong = 300;

// array with every 7-segment display
const int displayCount = 4;
int displaySegments[displayCount] = {
  segD1, segD2, segD3, segD4
};

// values of every 7-segment display
int displayValues[displayCount] = {
  0, 0, 0, 0
};

// encoding of each hex number form 0 to F
const int encodingsCount = 18;
byte byteEncodings[encodingsCount] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
  B11101110,  // A
  B00111110,  // b
  B10011100,  // C
  B01111010,  // d
  B10011110,  // E
  B10001110,  // F
};

int stateOne = 1;      // state one
int stateTwo = 2;      // state two
int state = stateOne;  // the current state

void setup() {
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displaySegments[i], OUTPUT);
    digitalWrite(displaySegments[i], HIGH);
  }

  Serial.begin(9600);
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swValue = digitalRead(pinSW);

  showValues();
  swPress();
  joyMoveState();
}

/* joy movement: Checks if the joystick has not moved by fitting the
values between two central thresholds. It considers the joystick to have moved 
only if its values reach the upper threshold and if it exceeded the bounce. 
Moving the joystick on the X-axis moves between screens, and the Y-axis 
increases or decreases the values of each screen. */
void joyMoveState() {
  if ((minThresholdCenter <= xValue && xValue <= maxThresholdCenter) and (minThresholdCenter <= yValue && yValue <= maxThresholdCenter)) {
    joyMoved = false;
  }

  if (xValue < minThresholdMargin && joyMoved == false) {
    axis = xAxis;
    direction = -1;
    joyMoved = true;
  }

  if (xValue > maxThresholdMargin && joyMoved == false) {
    axis = xAxis;
    direction = 1;
    joyMoved = true;
  }

  if (yValue < minThresholdMargin && joyMoved == false) {
    axis = yAxis;
    direction = -1;
    joyMoved = true;
  }

  if (yValue > maxThresholdMargin && joyMoved == false) {
    axis = yAxis;
    direction = 1;
    joyMoved = true;
  }

  if (lastJoyMoved != joyMoved) {
    lastDebounceTimeJoy = millis();
  }

  if (millis() - lastDebounceTimeJoy > debounceDelayJoy) {
    if (joyMoved != currentJoyMove) {
      currentJoyMove = joyMoved;

      if (currentJoyMove == true) {

        // joystick movement on the X axis
        if (state == stateOne && axis == xAxis) {
          currentDisplay = currentDisplay + direction;

          // create a cycle between screens
          if (currentDisplay == 4) {
            currentDisplay = 0;
          } else if (currentDisplay == -1) {
            currentDisplay = 3;
          }

          // joystick movement on the Y axis
        } else if (state == stateTwo && axis == yAxis) {
          displayValues[currentDisplay] += direction;

          // create a cycle between hex values
          if (displayValues[currentDisplay] == 16) {
            displayValues[currentDisplay] = 0;
          } else if (displayValues[currentDisplay] == -1) {
            displayValues[currentDisplay] = 15;
          }
        }
      }
    }
  }

  lastJoyMoved = joyMoved;
}

/* for the short press of the button goes from one state to another (from
state 1 to state 2 and vice versa). For long press of the button in state 1,
reset the digits to zero and set the current digit as the and the current 
position is set to the first (right most) digit, in the first state. */
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
          if (state == stateOne) {
            state = stateTwo;
          } else {
            state = stateOne;
          }
        } else {
          if (state == stateOne) {
            resetValues();
          }
        }
      }
    }
  }

  lastSwValue = swValue;
}

/* displaying the values of each screen by turning it on and off repeatedly. To 
blink the Decimal Point of the screen we are on, we add 1 (DP on) or 
not (DP off) to the hex value of the current screen. */
void showValues() {
  for (int displayNumber = 0; displayNumber < displayCount; displayNumber++) {
    byte encoding = byteEncodings[displayValues[displayNumber]];

    if (displayNumber == currentDisplay) {
      if (state == stateOne) {
        if (millis() - previousBlink >= intervalBlink) {
          previousBlink = millis();
          onOffBlink = !onOffBlink;
        }
        encoding += onOffBlink;
      } else {
        encoding += 1;
      }
    }

    writeReg(encoding);
    activateDisplay(displayNumber);
    delay(5);
  }
}

/* constantly activate and deactivate screens aka "display refresh" */
void activateDisplay(int displayNumber) {
  for (auto display : displaySegments) {
    digitalWrite(display, HIGH);
  }
  digitalWrite(displaySegments[displayNumber], LOW);
}

/* write register using dataPin, clockPin, latchPin and hex coded value */
void writeReg(byte encoding) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);
  digitalWrite(latchPin, HIGH);
}

/* all the digits go back to 0 and the current position is set to the first
(rightmost) digit */
void resetValues() {
  for (int i = 0; i <= 3; i++) {
    displayValues[i] = 0;
    currentDisplay = 0;
  }
}