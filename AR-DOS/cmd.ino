//WARNING! : Control is a feature in testing, and it will be used later with the addition of multitasking.
//Right now it's useless, and it only moves GRAPHICALLY the cursor around the screen, messing with the command line

void cmd() {  //Command-line based shell

  printDir(currentPath);
  print(F(">"));

  bool enter = false, cycling = false, cursor = true, shift = false, control = false;
  uint8_t keyCount = 0;
  uint16_t lineX = tft.getCursorX(), lineY = tft.getCursorY();  //Sarting position of the command line
  char keyCurrent = ' ', keyLast = ' ';
  inputSaved = '\0';
  unsigned long start = millis(), end;

  while (!enter) {  //Input //I wrote this code when I was in school, and i have no idea how it works anymore, so I'm not touching anything!

    char keyPressed = pad.getKey();
    if (keyPressed) {
      cursor = true;                     //Makes the cursor visible whenever a key is pressed
      start = millis();                  //Resets countdown of the cursor, essentialy freezing it in the current state
      tone(speakerPin, speakerHz, 100);  //BEEP
    }

    switch (keyPressed) {
      default:  //Printable chars
        if (cycling) {
          if (keyCount < 3) keyCount++;
          else keyCount = 0;
        } else cycling = true;

        if (keyPressed != keyLast) {
          keyCount = 0;
          keyLast = keyPressed;
        }

        if (!control) {                                                   //WARNING! : Control is a feature in testing, and it will be used later with the addition of multitasking. Right now it's useless
          if (!shift) keyCurrent = keyNormal[keyPressed - 48][keyCount];  //keyPressed - 48 converts a char to a number ('0' = 48)
          else keyCurrent = keyShift[keyPressed - 48][keyCount];
        }
        break;

      case '#':  //Enter
        if (!control) {
          if (cycling) {
            cycling = false;
            inputSaved += keyCurrent;
            print(keyCurrent, true, false);
          } else enter = true;
          keyLast = keyPressed;
          keyCurrent = ' ';
        } else {  //Not a feature, it's just for fun
          print(textBuffer[tft.getCursorX() / 6][tft.getCursorY() / 8], true, false);
        }
        break;

      case '*':  //Backspace //Need to fix text warp bug
        if (!control) {
          if (cycling) {
            print(F(" "));
            tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
            cycling = false;
          } else if (inputSaved.length() != 0) {
            inputSaved.remove(inputSaved.length() - 1);
            tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
            print(F("  "));
            tft.setCursor(tft.getCursorX() - 12, tft.getCursorY());
          }
          keyLast = keyPressed;
          keyCurrent = ' ';
        } else {
          print(textBuffer[tft.getCursorX() / 6][tft.getCursorY() / 8], true, false);
          tft.setCursor(tft.getCursorX() - 12, tft.getCursorY());
        }
        break;

      case 'A':  //Shift
        shift = !shift;
        if (cycling) {
          if (!shift) keyCurrent = keyNormal[keyLast - 48][keyCount];  //keyPressed - 48 converts a char to a number ('0' = 48)
          else keyCurrent = keyShift[keyLast - 48][keyCount];
        }
        break;

      case 'B':  //Control
        control = !control;
        break;

      case 'C':  //Recall
        if (!control) {
          inputSaved = inputLast;                                                       //Replaces the current input with the saved command
          tft.fillRect(lineX, lineY, 320, 8, colorBack);                                //Clears line
          for (uint16_t i = lineX / 6; i < charX; i++) textBuffer[i][lineY / 8] = ' ';  //Clears buffer
          tft.setCursor(lineX, lineY);                                                  //Sets cursor to start of the command line
          print(inputSaved);                                                            //Prints the saved command
          keyLast = keyPressed;
        } else {
          print(textBuffer[tft.getCursorX() / 6][tft.getCursorY() / 8], true, false);
          tft.setCursor(tft.getCursorX() - 6, tft.getCursorY() - 8);
        }
        break;

      case 'D':  //Debug
        if (!control) {
          inputSaved = "dbg";
          enter = true;
        } else {
          print(textBuffer[tft.getCursorX() / 6][tft.getCursorY() / 8], true, false);
          tft.setCursor(tft.getCursorX() - 6, tft.getCursorY() + 8);
        }
        keyLast = keyPressed;
        break;
      case '\0':  //Nothing pressed
        break;
    }

    if (Serial.available() > 0 && !enter) {  //As an alternative to keypad, read from serial
      inputSaved = Serial.readString();
      inputSaved.trim();
      print(keyCurrent, true, false);
      tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
      print(inputSaved);
      enter = true;
    }

    end = millis();  //For blinking cursor
    if (end - start > 500) {
      cursor = !cursor;
      start = millis();
    }

    if (!enter) {  //Cursor //Need to fix a lot of cursors bugs (text warp...) //And this sintax isn't helping (i wrote it like this)

      if (control) print(textBuffer[tft.getCursorX() / 6][tft.getCursorY() / 8], true, true);  //Not a feature, it's just for fun

      else if (cursor) {
        if (cycling) print(keyCurrent, true, true);
        else print('_');
      } else print(keyCurrent, true, false);

      tft.setCursor(tft.getCursorX() - 6, tft.getCursorY());
    } else println(keyCurrent, true, false);
  }

  inputLast = inputSaved;  //For recall button
  frag(parameters);
  inputFrag[0].toUpperCase();  //Commands are always uppercase

  printed = false;

  if (inputFrag[0] == "CMD") cmd();
  else if (inputFrag[0] == "CMP") cmp(inputFrag[1], inputFrag[2]);
  else if (inputFrag[0] == "CD") cd(inputFrag[1]);
  else if (inputFrag[0] == "RUN") run(inputFrag[1]);
  else if (inputFrag[0] == "MAKE") make(inputFrag[1]);
  else if (inputFrag[0] == "MD") md(inputFrag[1]);
  else if (inputFrag[0] == "EDIT") edit(inputFrag[1], inputFrag[2]);
  else if (inputFrag[0] == "READ") type(inputFrag[1]);
  else if (inputFrag[0] == "DEL") del(inputFrag[1]);
  else if (inputFrag[0] == "TREE") tree();
  else if (inputFrag[0] == "CLS") cls();
  else if (inputFrag[0] == "ECHO") echo(inputFrag[1]);
  else if (inputFrag[0] == "SET") set(inputFrag[1], inputFrag[2]);
  else if (inputFrag[0] == "DBG") dbg();
  else if (inputFrag[0] == "EXIT") exit();
  else if (inputFrag[0] != "") print(F("Bad command or file name"));

  if (inputFrag[0] != "") {
    if (printed) println('\n');
    else println();
  }
}