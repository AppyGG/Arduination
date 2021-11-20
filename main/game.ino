void game() {
  if (!gameInitialized) {
    displayInGame();
  }

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
        digitalWrite(ledPinB, HIGH);
        digitalWrite(ledPinR, LOW);
        saveRunnningGameScores();
      } else if (stateLED == 1) { // Red
        countR++;
        digitalWrite(ledPinR, HIGH);
        digitalWrite(ledPinB, LOW);
        saveRunnningGameScores();        
      }
      // Update display only if score changes
      displayInGame();
      counterPoint = 0;
    } else {
      counterPoint++;
    }
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    timer++;
    if (menuState == 1) {
      displayInGame();
    }
  }
}

void paused() {
  displayInGame();
  lcd.setCursor(14, 1);
  lcd.write(byte(0));
  digitalWrite(ledPinR, LOW);
  digitalWrite(ledPinB, LOW);
}

void saveRunnningGameScores() {
  EEPROM.update(GameRunningADDR, 1);
  if (menuState == 2) {
    if (savingInc == 0) {
        writeLongIntoEEPROM(GameTimerADDR, timer);
        writeUnsignedIntIntoEEPROM(BlueScoreADDR, countB);
        writeUnsignedIntIntoEEPROM(RedScoreADDR, countR);
        EEPROM.update(ScoringTeamADDR, stateLED);
        savingInc = 0;
    }
  } else if (menuState == 3) {
    if (savingInc == 10) {
      writeLongIntoEEPROM(GameTimerADDR, (timer / 10));
      writeUnsignedIntIntoEEPROM(BlueScoreADDR, countB);
      writeUnsignedIntIntoEEPROM(RedScoreADDR, countR);
      EEPROM.update(ScoringTeamADDR, stateLED);
      savingInc = 0;
    } else{
      savingInc++;
    }
  }
}

void eraseRunningGameScores() {
  if (countB + countR > 0){
    EEPROM.update(GameRunningADDR, 0);
    writeUnsignedIntIntoEEPROM(SavedBlueScoreADDR, countB);
    writeUnsignedIntIntoEEPROM(SavedRedScoreADDR, countR);
    writeLongIntoEEPROM(SavedGameTimerADDR, timer);
    EEPROM.update(ScoringTeamADDR, 0);
    timer = 0;
    countB = 0;
    countR = 0;
    stateLED = 2;
    menuState = 0;
    savingInc = 0;
  }
}
