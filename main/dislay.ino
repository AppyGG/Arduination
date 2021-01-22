void displayMenu() {
    lcd.home();
    if (menuState == 0) {
      lcd.setCursor(1, 1);
      lcd.print(" ");
      lcd.setCursor(9, 1);
      lcd.print("  ");
      lcd.setCursor(3, 0);
      lcd.print("Scores");
      lcd.setCursor(3, 1);
      lcd.print("Points");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      // Scrollbar
      lcd.setCursor(15, 0);
      lcd.write(byte(1));
      lcd.setCursor(15, 1);
      lcd.write(byte(2));
      // Cursor position
      lcd.setCursor(1, 0);
    } else if (menuState == 1) {
      lcd.setCursor(1, 0);
      lcd.print(" ");
      lcd.setCursor(3, 0);
      lcd.print("Scores");
      lcd.setCursor(3, 1);
      lcd.print("Points");
      // Scrollbar
      lcd.setCursor(15, 0);
      lcd.write(byte(3));
      lcd.setCursor(15, 1);
      lcd.write(byte(2));
      // Cursor position
      lcd.setCursor(1, 1);
    } else if (menuState == 2) {
      lcd.setCursor(3, 0);
      lcd.print("Points");
      lcd.setCursor(3, 1);
      lcd.print("Timer");
       // Scrollbar
      lcd.setCursor(15, 0);
      lcd.write(byte(5));
      lcd.setCursor(15, 1);
      lcd.write(byte(4));
      // Cursor position
      lcd.setCursor(1, 1);
    } else if (menuState == 3) {
      lcd.setCursor(3, 0);
      lcd.print("Timer");
      lcd.setCursor(3, 1);
      lcd.print("Settings");
       // Scrollbar
      lcd.setCursor(15, 0);
      lcd.write(byte(5));
      lcd.setCursor(15, 1);
      lcd.write(byte(6));
      // Cursor position
      lcd.setCursor(1, 1);
    }
    lcd.print(">");
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
    if (menuState == 1) {
      lcd.print(F("Blue  "));  // Print blue team
      lcd.print(countB);
      lcd.setCursor(11, 0); // Print timer
      lcd.print(secondsToHMS(timer));
      lcd.setCursor(0, 1);
      lcd.print(F("Red   "));  // Print red team
      lcd.print(countR);
    } else if (menuState == 2) {
      lcd.print(F("Blue  "));  // Print blue team
      lcd.print(secondsToHMS(countB));
      //lcd.setCursor(11, 0); // Print timer
      //lcd.print(secondsToHMS(timer));
      lcd.setCursor(0, 1);
      lcd.print(F("Red   "));  // Print red team
      lcd.print(secondsToHMS(countR));
    }
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

void settings() {
    lcd.clear();
    lcd.home();
    lcd.print("Settings TODO");
}
