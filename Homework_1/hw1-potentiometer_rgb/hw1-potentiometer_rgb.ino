const int maxAnalogValue = 1023;
const int minAnalogValue = 0;
const int maxVoltageValue = 255;
const int minVoltageValue = 0;

// input pins for reading values from potentiometers
const int redInputPin = A0;
const int greenInputPin = A1;
const int blueInputPin = A2;

// output pins for RGB LED
const int redOutputPin = 9;
const int greenOutputPin = 10;
const int blueOutputPin = 11;

void setup() {
  Serial.begin(9600);

  pinMode(redInputPin, INPUT);
  pinMode(greenInputPin, INPUT);
  pinMode(blueInputPin, INPUT);

  pinMode(redOutputPin, OUTPUT);
  pinMode(greenOutputPin, OUTPUT);
  pinMode(blueOutputPin, OUTPUT);
}

void loop() {
  // get the values received by the pins from the potentiometers
  int redAnalogValue = analogRead(redInputPin);
  int greenAnalogValue = analogRead(greenInputPin);
  int blueAnalogValue = analogRead(blueInputPin);

  // set RGB intensity values
  int redBrightness = map(redAnalogValue, minAnalogValue, maxAnalogValue, minVoltageValue, maxVoltageValue);
  int greenBrightness = map(greenAnalogValue, minAnalogValue, maxAnalogValue, minVoltageValue, maxVoltageValue);
  int blueBrightness = map(blueAnalogValue, minAnalogValue, maxAnalogValue, minVoltageValue, maxVoltageValue);

  // send RGB intensity values
  analogWrite(redOutputPin, redBrightness);
  analogWrite(greenOutputPin, greenBrightness);
  analogWrite(blueOutputPin, blueBrightness);
}
