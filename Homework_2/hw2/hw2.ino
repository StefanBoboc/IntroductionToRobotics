// pushbutton
const int buttonPin = 2;
byte currentButtonState = HIGH;
byte lastButtonState = HIGH;

// buzzer
const int buzzerPin = 12;
const int buzzerFrequencyState3 = 400;
const int buzzerDurationState3 = 400;
const long intervalBuzzState3 = 1000;
unsigned long previousBuzzState3 = 0;

const int buzzerFrequencyState4 = 400;
const int buzzerDurationState4 = 100;
const long intervalBuzzState4 = 500;
unsigned long previousBuzzState4 = 0;

// traffic light for cars
const int greenCarLedPin = 8;
const int yellowCarLedPin = 9;
const int redCarLedPin = 10;
byte greenCarLedState;
byte yellowCarLedState;
byte redCarLedState;

// traffic light for pedestrians
const int greenPedLedPin = 4;
const int redPedLedPin = 5;
byte greenPedLedState;
byte redPedLedState;

// last time when green led blinked and duration of the blinks for state 4
unsigned long previousBlink = 0;
const long intervalBlink = 500;

// the current state
int state = 1;

// duration of every state
const long delayState2 = 8000;
const long durationState2 = 3000;
const long durationState3 = 8000;
const long durationState4 = 4000;

// debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long lastStateTime;

/* state 1 (aka Default State): green light for cars, red light for pedestrians and no sounds.   
Duration: indefinite, changed by pressing the button. */
void state1() {
  greenCarLedState = HIGH;
  yellowCarLedState = LOW;
  redCarLedState = LOW;

  greenPedLedState = LOW;
  redPedLedState = HIGH;
}

/* state 2: yellow light for cars, still red light for pedestrians and no beeping sound.
Duration: starts with a delay of 8 seconds after the button is pressed and lasts for 3 seconds. */
void state2() {
  // delay process
  if ((millis() - lastStateTime) > delayState2) {
    greenCarLedState = LOW;
    yellowCarLedState = HIGH;

    // goes to state 3 after state 2 has finished
    if ((millis() - lastStateTime - delayState2) >= durationState2) {
      state = 3;
      lastStateTime = millis();
    }
  }
}

/* state 3: red light for cars, green light for pedestrians and beeping sound.
Duration: 8 seconds. */
void state3() {
  yellowCarLedState = LOW;
  redCarLedState = HIGH;

  greenPedLedState = HIGH;
  redPedLedState = LOW;

  beepingState3();

  // goes to state 4 after state 3 has finished
  if ((millis() - lastStateTime) >= durationState3) {
    state = 4;
    lastStateTime = millis();
  }
}

/* state 4: red light for cars, blinking green light for pedestrians and beep sound faster.
Duration: 4 seconds. */
void state4() {
  blinkGreenPedLed();
  beepingState4();

  // goes to state 1 after state 4 has finished
  if ((millis() - lastStateTime) >= durationState4) {
    state = 1;
  }
}

/* blinking green light for pedestrians */
void blinkGreenPedLed() {
  if ((millis() - previousBlink) >= intervalBlink) {
    // save the last time the LED blinked
    previousBlink = millis();

    // if the LED is off, turn it on and vice-versa
    if (greenPedLedState == LOW) {
      greenPedLedState = HIGH;
    } 
    else {
      greenPedLedState = LOW;
    }
  }
}

/* buzzing setings for state 3 */
void beepingState3() {
  if ((millis() - previousBuzzState3) >= intervalBuzzState3) {
    // save the last time the buzzer beeped
    previousBuzzState3 = millis();
    
    tone(buzzerPin, buzzerFrequencyState3, buzzerFrequencyState3);
  }
}

/* buzzing setings for state 4 */
void beepingState4() {
  if ((millis() - previousBuzzState4) >= intervalBuzzState4) {
    // save the last time the buzzer beeped
    previousBuzzState4 = millis();

    tone(buzzerPin, buzzerFrequencyState4, buzzerFrequencyState4);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(buzzerPin, OUTPUT);

  pinMode(greenCarLedPin, OUTPUT);
  pinMode(yellowCarLedPin, OUTPUT);
  pinMode(redCarLedPin, OUTPUT);

  pinMode(greenPedLedPin, OUTPUT);
  pinMode(redPedLedPin, OUTPUT);
}

void loop() {
  // read the state of the button into a local variable:
  int readingButtonValue = digitalRead(buttonPin);

  // if the button changed, due to noise or pressing
  if (readingButtonValue != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the button state has changed
    if (readingButtonValue != currentButtonState) {
      currentButtonState = readingButtonValue;

      // only start the process if the new button state is HIGH
      if (currentButtonState == HIGH && state == 1) {
        lastStateTime = millis();

        // goes to state 2
        state = 2;
      }
    }
  }

  switch (state) {
    case 1:
      state1();
      break;
    case 2:
      state2();
      break;
    case 3:
      state3();
      break;
    case 4:
      state4();
      break;
  }

  digitalWrite(greenCarLedPin, greenCarLedState);
  digitalWrite(yellowCarLedPin, yellowCarLedState);
  digitalWrite(redCarLedPin, redCarLedState);

  digitalWrite(greenPedLedPin, greenPedLedState);
  digitalWrite(redPedLedPin, redPedLedState);

  lastButtonState = readingButtonValue;
}