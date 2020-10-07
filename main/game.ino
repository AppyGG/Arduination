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
        digitalWrite(ledPinB, HIGH);
        digitalWrite(ledPinR, LOW);
      } else if (stateLED == 1) { // Red
        countR++;
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
    if (stateLED != 0) {
        digitalWrite(ledPinB, LOW);
      } else if (stateLED = 1) {
        digitalWrite(ledPinR, LOW);
    }
}
