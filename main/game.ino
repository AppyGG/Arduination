void game() {
  if (digitalRead(buttonPinB) == LOW && stateLED != 0) {
    stateLED = 0;
    counterPoint = 0;
    digitalWrite(ledPinB, HIGH);
    digitalWrite(ledPinR, LOW);
  } else if (digitalRead(buttonPinR) == LOW && stateLED != 1) {
    stateLED = 1;
    counterPoint = 0;
    digitalWrite(ledPinR, HIGH);
    digitalWrite(ledPinB, LOW);
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    if (counterPoint == pointInterval) {
      // LED Management
      if (stateLED == 0) { // Blue
        countB++;
        saveRunnningGameScores();
        digitalWrite(ledPinB, HIGH);
        digitalWrite(ledPinR, LOW);
      } else if (stateLED == 1) { // Red
        countR++;
        saveRunnningGameScores();
        digitalWrite(ledPinR, HIGH);
        digitalWrite(ledPinB, LOW);
      }
      counterPoint = 0;
    } else {
      counterPoint++;
    }
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    timer++;
    displayInGame();
  }
}

void paused() {
  displayInGame();
  lcd.setCursor(14, 1);
  lcd.write(byte(0));
}

void saveRunnningGameScores() {
  EEPROM.update(GameRunningADDR, 1);
  EEPROM.update(BlueScoreADDR, countB);
  EEPROM.update(RedScoreADDR, countR);
  EEPROM.update(GameTimerADDR, timer);
  EEPROM.update(ScoringTeamADDR, stateLED);
}

void eraseRunningGameScores() {
  if (countB + countR > 0){
    EEPROM.update(GameRunningADDR, 0);
    EEPROM.update(SavedBlueScoreADDR, countB);
    EEPROM.update(SavedRedScoreADDR, countR);
    EEPROM.update(SavedGameTimerADDR, timer);
    timer = 0;
    countB = 0;
    countR = 0;
  }
}
