#include "LiquidCrystal.h"

LiquidCrystal lcd(7,6,5,4,3,2);

// constants won't change. They're used here to set pin numbers
const byte buttonPinB = 8;     // blue button pin
const byte buttonPinR = 9;     // red button pin
const byte ledPinB =  10;      // blue LED pin
const byte ledPinR =  11;      // red LED pin
const byte btnOk = 12;         // btn OK for menus

// variables will change
unsigned int countB = 0;
unsigned int countR = 0;
unsigned int stateLED = 2;    // 0 = Blue |1 = Red | 2 = Both off (start position)
String textCountB = "";
String textCountR = "";

// Black button settings
unsigned long buttonTimer = 0;
const long longPressTime  = 750;
boolean buttonActive = false;
boolean longPressActive = false;

// Menu settings
unsigned int menuState = 0;
unsigned int generalStatus = 0;
bool gamePaused = false;
bool gameInitialized = false;

// Game timer
unsigned long timer = 0;
unsigned long previousMillis = 0;
const long interval = 1000;
unsigned int counterPoint = 0;
const byte pointInterval = 10;

// Pause char
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
  pinMode(buttonPinB, INPUT_PULLUP);
  pinMode(buttonPinR, INPUT_PULLUP);
  // initialize menu pushbutton as an input
  pinMode(btnOk, INPUT_PULLUP);  
}

void loop() {
  unsigned long currentMillis = millis();
  if (digitalRead(btnOk) == LOW) { // Pressed
    if (buttonActive == false) { // First time pressed, init short or long press detection
      buttonActive = true;
      buttonTimer = millis();
    }

    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) { // LongPressed
      longPressActive = true;
      if (generalStatus == 0) { // On menu
        menuValidate();
      } else if (generalStatus == 2) { // On game screen
        lcd.clear();
        gameInitialized = false;
        generalStatus = 0;
      } else if (generalStatus == 3) { // Pause screen
        lcd.clear(); 
        gameInitialized = false;
        timer = 0;
        generalStatus = 0;
      }
    }
  } else { // Not Pressed
    if (buttonActive == true) { // Button released
      if (longPressActive == true) { // Reset long pressed
        longPressActive = false;
      } else {
        if (generalStatus == 0) { // On menu
          menuNavigate();
        } else if (generalStatus == 2) { // On game screen
          generalStatus = 3;
        } else if (generalStatus == 3) { // Pause screen
          lcd.setCursor(14, 1);
          lcd.print(F(" "));
          generalStatus = 2;
        }
      }
      buttonActive = false;
    }
  }
  
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
