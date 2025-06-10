void input(bool save) {
  bool enter = false;
  bool cycling = false;
  bool shift = false;
  bool control = false;
  bool cursor = true;
  uint8_t keyCount = 0;
  char keyCurrent = ' ';
  char keyLast = ' ';
  String inputLast;
  inputSaved = '\0';
  unsigned long start = millis();
  unsigned long end;
  const String debugString = "run TRIS.EXE";

  while (!enter) {

    keyPressed = pad.getKey();
    if (keyPressed) tone(audioPin, audioHz, 100);

    switch (keyPressed) {
      default:
        if (cycling) {
          if (keyCount < 3) {
            keyCount++;
          } else {
            keyCount = 0;
          }
        } else cycling = true;

        if (keyPressed != keyLast) {
          keyCount = 0;
          keyLast = keyPressed;
        }

        if (!control) {
          if (!shift) {
            keyCurrent = keyNormal[keyPressed - 48][keyCount];  //keyPressed-48 trasforma un numero da carattere a intero
            print(keyCurrent, false);
          } else {
            keyCurrent = keyShift[keyPressed - 48][keyCount];
            print(keyCurrent, false);
          }
        }
        tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
        break;

      case '#':
        if (!control) {
          if (cycling) {
            cycling = false;
            inputSaved += keyCurrent;
            bg = true;
            print(keyCurrent, false);
          } else {
            enter = true;
          }
          keyLast = keyPressed;
          keyCurrent = ' ';
        } else {
          tft.setCursor(tft.getCursorX() + 6, tft.getCursorY());
        }
        break;

      case '*':
        if (!control) {
          if (cycling) {
            print(F(" "), false);
            tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
            cycling = false;
          } else if (inputSaved.length() != 0) {
            inputSaved.remove(inputSaved.length() - 1);
            tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
            print(F("  "), false);
            tft.setCursor(tft.getCursorX() - 12, tft.getCursorY());
          }
          keyLast = keyPressed;
          keyCurrent = ' ';
        } else tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
        break;

      case 'A':
        shift = !shift;
        break;

      case 'B':
        control = !control;
        break;

      case 'C':

        if (!control) {
          inputSaved = inputLast;
          print(inputLast, false);
          keyLast = keyPressed;
        } else tft.setCursor(tft.getCursorX(), tft.getCursorY() - 8);
        break;

      case 'D':
        if (!control) {
          inputSaved += debugString;
          print(debugString, false);
        } else tft.setCursor(tft.getCursorX(), tft.getCursorY() + 8);
        break;
      case '\0':
        break;
    }

    if (Serial.available() > 0) {
      inputSaved = Serial.readString();
      inputSaved.trim();
      print(inputSaved, false);
      enter = true;
    }

    end = millis();
    if ((end - start) > 500) {
      cursor = !cursor;
      start = millis();
    }
    if (!enter) {
      if (cursor) {
        if (cycling) {
          bg = true;
          invert = true;
          print(keyCurrent, false);
          tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
        } else {
          print(F("_"), false);
          tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
        }
      } else {
        bg = true;
        print(keyCurrent, false);
        tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
      }
    } else {
      bg = true;
      print(keyCurrent, true);
    }

    bg = false;
    invert = false;
  }

  if (save) inputLast = inputSaved;
}