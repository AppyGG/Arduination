#include "LiquidCrystal.h"
#include "EEPROM.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// constants won't change. They're used here to set pin numbers
const byte buttonPinB = 10;           // blue button pin
const byte buttonPinR = 11;           // red button pin
const byte ledPinB = 9;               // blue LED pin
const byte ledPinR = 8;               // red LED pin
const byte btnOk = 12;                // btn OK for menus
const int GameRunningADDR = 0;        // Byte address of running game status
const int BlueScoreADDR = 4;          // Byte address of running game blue score
const int RedScoreADDR = 8;           // Byte address of running game red score
const int ScoringTeamADDR = 12;       // Byte address of running game scoring team
const int GameTimerADDR = 16;         // Byte address of running game timer
const int SavedBlueScoreADDR = 20;    // Byte address of running game blue score
const int SavedRedScoreADDR = 24;     // Byte address of running game red score
const int SavedGameTimerADDR = 28;    // Byte address of running game duration

// variables will change
unsigned int countB = 0;
unsigned int countR = 0;
byte stateLED = 2;    // 0 = Blue |1 = Red | 2 = Both off (start position)

// Black button settings
unsigned long buttonTimer = 0;
const long longPressTime  = 750;
boolean buttonActive = false;
boolean longPressActive = false;

// Menu settings
unsigned int menuState = 0;
unsigned int generalStatus = 0; // 0 = MENU | 1 = SCORES | 2 = GAME | 3  = RESET TRIGGERED
bool gamePaused = false;
bool gameInitialized = false;
bool scoresDisplayed = false;

// Game timer
unsigned long timer = 0;
unsigned long previousMillis = 0;
unsigned long interval = 1000;
unsigned int counterPoint = 0;
const byte pointInterval = 10;
unsigned int savingInc = 0;    // Save score increment

// CUSTOM CHARs

// Pause char
byte pauseChar[8] = {
  B00000,
  B11011,
  B11011,
  B11011,
  B11011,
  B11011,
  B00000,
  B00000,
};

byte scrollBarTopChar1[8] = {
  B00111,
  B00011,
  B00011,
  B00011,
  B00000,
  B00000,
  B00000,
  B00000,
};

byte scrollBarTopChar2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00111,
};

byte scrollBarMid1Char[8] = {
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00011,
  B00011,
  B00011,
};

byte scrollBarMid2Char[8] = {
  B00011,
  B00011,
  B00011,
  B00000,
  B00000,
  B00000,
  B00000,
  B00111,
};

byte scrollBarBotChar1[8] = {
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};

byte scrollBarBotChar2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00011,
  B00011,
  B00011,
  B00111,
};

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps

  // Initialize scrollbar chars
  lcd.createChar(0, pauseChar);
  lcd.createChar(1, scrollBarTopChar1);
  lcd.createChar(2, scrollBarTopChar2);
  lcd.createChar(3, scrollBarMid1Char);
  lcd.createChar(4, scrollBarMid2Char);
  lcd.createChar(5, scrollBarBotChar1);
  lcd.createChar(6, scrollBarBotChar2);
  
  lcd.begin(16, 2); // Initialize LCD
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

  //if (EEPROM.read(GameRunningADDR) == 1) {
  //  countB = readUnsignedIntFromEEPROM(BlueScoreADDR);
  //  countR = readUnsignedIntFromEEPROM(RedScoreADDR);
  //  stateLED = EEPROM.read(ScoringTeamADDR);
  //  timer  = readLongFromEEPROM(GameTimerADDR);
  //  if (stateLED == 0) {
  //    digitalWrite(ledPinB, HIGH);
  //  } else if (stateLED == 1) {
  //    digitalWrite(ledPinR, HIGH);
  //  }
  //  generalStatus = 2;
  //}
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
      if (generalStatus == 0) { // On menu : VALIDATE
        menuValidate();
      } else if (generalStatus == 1) { // On scores : RETURN MENU
        lcd.clear();
        scoresDisplayed = false;
        generalStatus = 0;
      } else if (generalStatus >= 2) { // Pause screen : QUIT GAME / RESET SCORES
        lcd.clear();
        gameInitialized = false;
        generalStatus = 0;
      }
    }
  } else { // Not Pressed
    if (buttonActive == true) { // Button released
      if (longPressActive == true) { // Reset long pressed
        longPressActive = false;
      } else {
        if (generalStatus == 0) { // On menu : NAVIGATE MENU
          menuNavigate();
        } else if (generalStatus == 1) { // On scores : QUIT SCORES

        } else if (generalStatus == 2) { // On game screen : PAUSE GAME
          generalStatus = 3;
        } else if (generalStatus == 3) { // Pause screen : UNPAUSE GAME
          lcd.setCursor(14, 1);
          lcd.print(F(" "));
          generalStatus = 2;
        }
      }
      buttonActive = false;
    }
  }

  switch (generalStatus) {
    case 0:
      displayMenu();
      resetLEDs();
      break;
    case 1:
      if (!scoresDisplayed) {
        lcd.clear();
        scoresDisplayed = true;
      }
      displayScores();
      break;
    case 2:
      game();
      break;
    case 3:
      paused();
      break;
    case 4:
      reset();
      break;
    case 5:
      settings();
      break;
  }

//  ==================
//  ==  DEBUG CODE  ==
//  ==================
//  lcd.setCursor(12, 1);
//  lcd.print(menuState);
//  if (digitalRead(buttonPinB)) {
//    lcd.print('B');
//  } else {
//    lcd.print('.');
//  }
// if (digitalRead(btnOk)) {
//    lcd.print('N');
//  } else {
//    lcd.print('.');
//  }
//  if (digitalRead(buttonPinR)) {
//    lcd.print('R');
//  } else {
//    lcd.print('.');
//  }
//  lcd.setCursor(0, 0);
}

void menuNavigate() {
  if (menuState == 0) {
    menuState = 1;
  } else if (menuState == 1) {
    menuState = 2;
  } else if (menuState == 2) {
    menuState = 3;
  } else if (menuState == 3) {
  //   menuState = 4;
  // } else {
    menuState = 0;
  }
}

void menuValidate() {
  if (menuState == 0) { // Scores
    generalStatus = 1;
  } else if (menuState == 1) { // Start gamemode Points
    interval = 1000;
    generalStatus = 2;
  } else if (menuState == 2) { // Start gamemode Timer
    interval = 100;
    generalStatus = 2;
  } else if (menuState == 3) { // Reset
    generalStatus = 4;
  // } else { // Settings menu activated
    // generalStatus = 5;
  }
}

void resetLEDs() {
  digitalWrite(ledPinB, LOW);
  digitalWrite(ledPinR, LOW);
}

void reset() {
  lcd.clear();
  lcd.home();
  lcd.setCursor(2, 0);
  lcd.print(F("RESETTING"));

  EEPROM.update(GameRunningADDR, 0);

  delay(200);
  lcd.setCursor(11, 0);
  lcd.print(F("."));

  gameInitialized = false;

  generalStatus = 0; // Back to main menu
  menuState = 0;     // Scroll top 

  delay(200);
  lcd.setCursor(13, 0);
  lcd.print(F("."));

  stateLED = 2; // Shut off leds
  countB = 0;   // Blue score to zero
  countR = 0;   // Blue score to zero

  lcd.setCursor(12, 0);
  lcd.print(F("."));
  delay(200);

  resetLEDs();

  lcd.clear();
}