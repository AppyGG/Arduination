
void displayMenu() {
    lcd.home();
    lcd.print("Menu");
    lcd.setCursor(7, 0);
    lcd.print("Scores");
    lcd.setCursor(7, 1);
    lcd.print("New game");
    if (menuState == 0) {
        lcd.setCursor(5, 1);
        lcd.print(" ");
        lcd.setCursor(5, 0);
    } else {
        lcd.setCursor(5, 0);
        lcd.print(" ");
        lcd.setCursor(5, 1);
    }
    lcd.print("*");
}

void displayInGame() {
    // Clear screen if a new game is started
    if (!gameInitialized) {
       gameInitialized = true;
       lcd.clear();
       delay(250);
    }
    lcd.home();           // Reset cursor
    lcd.setCursor(0, 0);
    lcd.print(F("Blue  "));  // Print blue team
    lcd.print(countB);
    lcd.setCursor(11, 0); // Print timer
    lcd.print(secondsToHMS(timer));
    lcd.setCursor(0, 1);
    lcd.print(F("Red   "));  // Print red team
    lcd.print(countR);
}

void displayScores() {
    lcd.home();
    lcd.print(F("SCORES"));
    lcd.setCursor(11,0);
    lcd.print(secondsToHMS(readLongFromEEPROM(SavedGameTimerADDR)));
    lcd.setCursor(0,1);
    lcd.print("B: ");
    lcd.print(readUnsignedIntFromEEPROM(SavedBlueScoreADDR));
    lcd.setCursor(8,1);
    lcd.print("R: ");
    lcd.print(readUnsignedIntFromEEPROM(SavedRedScoreADDR));
}
