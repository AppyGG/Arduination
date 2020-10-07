#include "LiquidCrystal.h"

LiquidCrystal lcd(7,6,5,4,3,2);

// constants won't change. They're used here to set pin numbers
const byte buttonPinB = 8;     // blue button pin
const byte buttonPinR = 9;     // red button pin
const byte ledPinB =  10;      // blue LED pin
const byte ledPinR =  11;      // red LED pin
const byte btnNext = 12;       // btn Next for menus
const byte btnOk = 13;         // btn OK for menus

// variables will change
bool stateB = false;      // variable for reading the buttonB status
unsigned int countB = 0;
bool stateR = false;      // variable for reading the buttonR status
unsigned int countR = 0;
unsigned int stateLED = 2;    // 0 = Blue |1 = Red | 2 = Both off (start position)
String textCountB = "";
String textCountR = "";

// button settings
unsigned int statusNext = 0;
unsigned int buttonLastState = LOW;
unsigned int buttonCurrentState;
unsigned int statusOk = 0;
unsigned long buttonPressedTime;
unsigned long buttonReleasedTime;
const int SHORT_PRESS_TIME = 1000;
const int LONG_PRESS_TIME  = 1000;

unsigned int menuState = 0;
unsigned int generalStatus = 0;
bool gamePaused = false;
bool gameInitialized = false;

unsigned long timer = 0;
unsigned long previousMillis = 0;
const long interval = 1000;
unsigned int counterPoint = 0;
const byte pointInterval = 10;

byte pauseChar[8] = {
  B00000,
  B11011,
  B11011,
  B11011,
  B11011,
  B11011,
  B00000,
};

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps

  lcd.createChar(0, pauseChar);
  lcd.begin(16,2);// Initialize LCD
  lcd.home();
  lcd.clear();
  
  // initialize the LED pin as an output
  pinMode(ledPinB, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  // initialize the pushbutton pin as an input
  pinMode(buttonPinB, INPUT);
  pinMode(buttonPinR, INPUT);
  // initialize menu pushbutton as an input
  pinMode(btnNext, INPUT_PULLUP);
  pinMode(btnOk, INPUT_PULLUP);  
}

void loop() {
  unsigned long currentMillis = millis();
  buttonCurrentState = digitalRead(btnOk);
  
  if( buttonLastState == HIGH && buttonCurrentState == LOW ) {
      buttonPressedTime = millis();
  } else if (buttonLastState == LOW && buttonCurrentState == HIGH && (currentMillis - buttonPressedTime) < LONG_PRESS_TIME) { // Only trigger when released TODO : change that
      buttonReleasedTime = millis();

      if(generalStatus == 0){ // On menu
          //if( pressDuration < SHORT_PRESS_TIME ) { // Nav cycle
              menuNavigate();
          //}
      } else if (generalStatus == 2) { // On game screen
          //if( pressDuration < SHORT_PRESS_TIME ) { // Pause cycle
              generalStatus = 3;
          //}
      } else if (generalStatus == 3) { // Pause screen
          //if( pressDuration < SHORT_PRESS_TIME ) { // Unpause game
              lcd.setCursor(14, 1);
              lcd.print(F(" "));
              generalStatus = 2;
          //}
      }
  } else if (buttonLastState == LOW && buttonCurrentState == LOW && (currentMillis - buttonPressedTime) > LONG_PRESS_TIME) { // Long press not yet released
       if(generalStatus == 0){ // On menu
          //if ( pressDuration > LONG_PRESS_TIME ) { // Nav ok
              menuValidate();
          //}
      } else if (generalStatus == 2) { // On game screen
          //if ( pressDuration > LONG_PRESS_TIME ) { // Long press when game on (quit game ?)
              lcd.clear();
              gameInitialized = false;
              generalStatus = 0;
          //}
      } else if (generalStatus == 3) { // Pause screen
          //if ( pressDuration > LONG_PRESS_TIME ) { // Quit game
              lcd.clear(); 
              gameInitialized = false;
              timer = 0;
              generalStatus = 0;
          //}
      }
  }

  buttonLastState = buttonCurrentState;

  switch(generalStatus) {
    case 0:
      displayMenu();
      resetLEDs();
      break;
    case 1:
      scores();
      break;
    case 2:
      game();
      break;
    case 3:
      paused();
      break;
  }
}

void menuNavigate() {
    if (menuState == 1) {
       menuState = 0;
    } else {
       menuState = 1;
    }
}

void menuValidate() {
    if (menuState == 0) {
       generalStatus = 1;
    } else {
       generalStatus = 2;
    }
}

void resetLEDs() {
    digitalWrite(ledPinB, LOW);
    digitalWrite(ledPinR, LOW);
}
