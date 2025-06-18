bool loadedGui = false;

void cmd() {
  if (loadedGui) {

    print(diskSel + ":", false);
    printDir(currentPath);
    print(F(">"), false);

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

    while (!enter) {  //Input

      char keyPressed = pad.getKey();
      if (keyPressed) tone(audioPin, audioHz, 100);  //BEEP

      switch (keyPressed) {
        default:  //Printable chars //Need to fix text warp bug
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
              keyCurrent = keyNormal[keyPressed - 48][keyCount];  //keyPressed - 48 converts a char to a number ('0' = 48)
              print(keyCurrent, false);
            } else {
              keyCurrent = keyShift[keyPressed - 48][keyCount];
              print(keyCurrent, false);
            }
          }
          tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
          break;

        case '#':  //Enter
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

        case '*':  //Backspace //Need to fix text warp bug
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

        case 'A':  //Shift
          shift = !shift;
          break;

        case 'B':  //Control
          control = !control;
          break;

        case 'C':  //Recall or Up //Need to fix recall last input bug

          if (!control) {
            inputSaved = inputLast;
            print(inputLast, false);
            keyLast = keyPressed;
          } else tft.setCursor(tft.getCursorX(), tft.getCursorY() - 8);
          break;

        case 'D':  //Debug or Down
          if (!control) {
            inputSaved += debugString;
            print(debugString, false);
          } else tft.setCursor(tft.getCursorX(), tft.getCursorY() + 8);
          break;
        case '\0':  //Nothing pressed
          break;
      }

      if (Serial.available() > 0) {  //As an alternative to keypad, read from serial
        inputSaved = Serial.readString();
        inputSaved.trim();
        bg = true;  //YAY Fixed the cursor bug
        print(keyCurrent, false);
        tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
        bg = false;
        print(inputSaved, false);
        enter = true;
      }

      end = millis();  //For blinking cursor
      if ((end - start) > 500) {
        cursor = !cursor;
        start = millis();
      }

      if (!enter) {  //Cursor //Need to fix a lot of cursors bugs (text warp...)
        if (cursor) {
          if (cycling) {  //Highlites the cycling char
            bg = true;
            invert = true;
            print(keyCurrent, false);
            tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
          } else {  //W
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

    inputLast = inputSaved;
    frag(3);
    inputFrag[0].toUpperCase();

    printed = false;

    if (inputFrag[0] == "CMD") cmd();
    else if (inputFrag[0] == "CMP") cmp(inputFrag[1], inputFrag[2]);
    else if (inputFrag[0] == "CD") cd(inputFrag[1]);
    else if (inputFrag[0] == "RUN") run(inputFrag[1]);
    else if (inputFrag[0] == "MAKE") make(inputFrag[1]);
    else if (inputFrag[0] == "EDIT") edit(inputFrag[1], inputFrag[2]);
    else if (inputFrag[0] == "READ") read(inputFrag[1]);
    else if (inputFrag[0] == "DEL") del(inputFrag[1]);
    else if (inputFrag[0] == "TREE") tree();
    else if (inputFrag[0] == "CLS") cls();
    else if (inputFrag[0] == "ECHO") echo(inputFrag[1]);
    else if (inputFrag[0] == "SET") set(inputFrag[1], inputFrag[2]);
    else if (inputFrag[0] == "DBG") dbg();
    else if (inputFrag[0] == "EXIT") exit();
    else if (inputFrag[0] == "")
      ;
    else print(F("Command not found"), false);

    if (inputFrag[0] != "")
      if (printed) print('\n', true);
      else print(true);

  } else {
    tone(audioPin, audioHz, 250);
    print(F("\nAR-DOS 1.1\n"), true);  //Name and version
    loadedGui = true;
  }
}