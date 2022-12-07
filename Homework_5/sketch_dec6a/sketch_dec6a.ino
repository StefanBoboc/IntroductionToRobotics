/*
0123456789012345
   Welcome to   
 CATCH THE EGGS 

0123456789012345
      MENU    

0123456789012345
-   
*/

/*
0123456789012345
      MENU     
>Start game    +
>Highscore     +
>Settings      +
>About         +
>How to play   +
-----
0123456789012345
   HIGHSCORE  
1 STEFAN 99999 +
2 Play2  99999 +
3 Play3  99999 +
4 Play4  99999 +
5 Play5  99999 +
-----
0123456789012345
    SETTINGS
>Name: STEFAN  +
>Difficulty: 3 +
>LCD contrs: 9 +
>LCD bright: 5 +
>Mat bright: 5 +
>Sound: ON/OFF +
-----
0123456789012345
     ABOUT
Stefan Boboc
CATCH THE EGG 
StefanBoboc git
-----
0123456789012345
  HOW TO PLAY
Descriere...
*/
#include <LiquidCrystal.h>
byte INDICATOR_IMG[8] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11100,
  B11000,
  B10000,
  B00000
};

byte TOP_SCROLLBAR[8] = {
  0b11111,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

byte MIDDEL_SCROLLBAR[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

byte BOTTOM_SCROLLBAR[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b11111
};

byte EDIT_MODE[8] = {
  B00100,
  B00100,
  B00100,
  B11100,
  B11100,
  B00100,
  B00100,
  B00100
};

const int JOYSTICK_X = A0;
const int JOYSTICK_Y = A1;
const int JOYSTICK_SW = 2;

const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//---------------------

String welcomeMsg[] = { "Welcome to", "CATCH THE EGGS" };

const int menuItemsCount = 5;
String menuItems[menuItemsCount] = { "Start Game", "Highscore", "Settings", "About", "How to play" };
int menuItemPosition = 0;

const int settingsItemsCount = 6;
String settingsItems[settingsItemsCount] = { "Name:", "Difficulty:", "LCD contrs:", "LCD bright:", "Mat bright:", "Sound:" };
int settingsItemPosition = 0;

const int howToItemsCount = 6;
String howToItems[howToItemsCount] = { "Lorem Ipsum is", "simply dummy", "text of the", "printing and", "typesetting", "industry." };
int howToItemPosition = 0;

int joystickX;
int joystickY;
byte joystickSW;

int readJoystickX;
int readJoystickY;
byte readJoystickSW;

void gameIntro() {
  lcd.setCursor(3, 0);
  for (int i = 0; i < 10; i++) {
    lcd.print(welcomeMsg[0][i]);

    delay(300);
  }

  lcd.setCursor(1, 1);
  lcd.print(welcomeMsg[1]);

  delay(3000);

  lcd.clear();
}

void joystickReading() {
  readJoystickX = analogRead(JOYSTICK_X);
  readJoystickY = analogRead(JOYSTICK_Y);
  readJoystickSW = digitalRead(JOYSTICK_SW);

  if (400 <= readJoystickX && readJoystickX <= 600) {
    joystickX = 0;
  } else if (readJoystickX < 400) {
    joystickX = 1;
  } else {
    joystickX = -1;
  }

  if (400 <= readJoystickY && readJoystickY <= 600) {
    joystickY = 0;
  } else if (readJoystickY < 400) {
    joystickY = -1;
  } else {
    joystickY = 1;
  }

  joystickSW = !readJoystickSW;
}

void displayMenuTitle(String title) {
  int pos = (16 - title.length()) / 2;
  lcd.setCursor(pos, 0);
  lcd.print(title);
}

void displayMenuScrollbar(int itemsCount, int item) {
  lcd.setCursor(15, 1);

  if (item == 0) {
    lcd.write(byte(2));
  } else if (item == itemsCount - 1) {
    lcd.write(byte(4));
  } else {
    lcd.write(byte(3));
  }
}

void displayMenuIndicator(bool indicator) {
  if (indicator == true) {
    lcd.setCursor(0, 1);
    lcd.write(byte(1));
  }
}

void displayMenu() {
  lcd.clear();
  displayMenuTitle("MENU");
  displayMenuIndicator(true);
  lcd.print(menuItems[menuItemPosition]);
  displayMenuScrollbar(menuItemsCount, menuItemPosition);
}

void menuManager() {

}

void menuSetup(){

}

void menuManager(){
  joystickInput = menuAxisInput(joystickY);
}

void displaySettings() {
  lcd.clear();
  displayMenuTitle("SETTINGS");
  displayMenuIndicator(true);
  lcd.print(settingsItems[settingsItemPosition]);
  displayMenuScrollbar(settingsItemsCount, settingsItemPosition);
}

char aboutText[] = "CATCH THE EGGS by Stefan Boboc, StefanBoboc on GitHub / ";
char textPice[16] = "                ";
int j = 0;

void aboutMessage(){
  for (int i = 0; i < 15; i++) {
    textPice[i] = textPice[i + 1];
  }

  textPice[15] = aboutText[j];
  j++;
  if (j > 55) j = 0;
  lcd.setCursor(0, 1);
  lcd.print(textPice);
  delay(500);  
}

void aboutSetup() {
  lcd.clear();
  displayMenuTitle("ABOUT");
  
}
int lock = 0;
void aboutManager(){
  switch(lock){
    case 0:
      aboutSetup();
      lock = 1;
      break;
    case 1:
      aboutMessage();
      break;

  }

}



bool joystickMoved = false;
int menuAxisInput(int axisValue) {
  int input = axisValue;
  if (input == 0) {
    joystickMoved = false;
    return 0;
  } else if (joystickMoved == false) {
    joystickMoved = true;
    return input;
  } else {
    return 0;
  }
}

bool lastJoystickSW = LOW;
bool currentJoystickSW = LOW;
unsigned long lastDebounceTimeSW = 0;
unsigned long debounceDelaySW = 100;
int menuSWInput() {
  if (joystickSW == HIGH and lastJoystickSW == LOW) {
    lastDebounceTimeSW = millis();
  }

  if (millis() - lastDebounceTimeSW > debounceDelaySW) {
    if (joystickSW != currentJoystickSW) {
      currentJoystickSW = joystickSW;

      if (currentJoystickSW == HIGH) {
      }
    }
  }

  lastJoystickSW = joystickSW;
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(JOYSTICK_SW, INPUT_PULLUP);

  lcd.createChar(1, INDICATOR_IMG);
  lcd.createChar(2, TOP_SCROLLBAR);
  lcd.createChar(3, MIDDEL_SCROLLBAR);
  lcd.createChar(4, BOTTOM_SCROLLBAR);

  //gameIntro();
}

void loop() {
  joystickReading();
  //menuAxisInput();
  //menuSWInput();
  switch (state):
    case 0:
      menuManager();
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;


  int input = menuAxisInput(joystickY);

  if (input != 0) {
    /*  
    menuItemPosition = menuItemPosition - input;
    switch (menuItemPosition) {
      case -1:
        menuItemPosition = 0;
        break;
      case 5:
        menuItemPosition = 4;
        break;
    }
    displayMenu();
    
    */

    /*
    settingsItemPosition = settingsItemPosition - input;
    switch (settingsItemPosition) {
      case -1:
        settingsItemPosition = 0;
        break;
      case 6:
        settingsItemPosition = 5;
        break;
    }
    
    displaySettings();
    */
    /*
    aboutItemPosition = aboutItemPosition - input;
    switch (aboutItemPosition) {
      case -1:
        aboutItemPosition = 0;
        break;
      case 6:
        aboutItemPosition = 5;
        break;
    }
    */

    
  }
  aboutManager();
}

