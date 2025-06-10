int16_t screenBufferCountX, screenBufferCountY;
char screenBufferChar;

void frag(uint8_t par) {
  int charCount = 0;
  for (uint8_t parCurrent = 0; parCurrent < par; parCurrent++) inputFrag[parCurrent] = "";
  for (uint8_t parCurrent = 0; parCurrent < par; parCurrent++) {
    bool withSpace = false;
    if (parCurrent != 0) charCount++;  //skip the space
    while (inputSaved.charAt(charCount) != ' ' && !withSpace && charCount < inputSaved.length()) {
      if (inputSaved.charAt(charCount) == '"' && !withSpace) {
        charCount++;
        withSpace = true;
        while (inputSaved.charAt(charCount) != '"' && charCount < inputSaved.length()) {
          inputFrag[parCurrent] += inputSaved.charAt(charCount);
          charCount++;
        }
        charCount++;
      }
      if (!withSpace) {
        inputFrag[parCurrent] += inputSaved.charAt(charCount);
        charCount++;
      }
    }
  }
}

void error(int8_t type, String name, String file, bool ln) {
  switch (type) {
    default:
      print(F("Exit code: "), false);
      print(type, ln);
      break;
    case 0:
      print(name, false);
      print(F(" \""), false);
      print(file, false);
      print(F("\"..."), ln);  //tre pallini = sta facendo un'operazione
      break;
    case 1:
      print(name, false);
      print(F(" \""), false);
      print(file, false);
      print(F("\"!"), ln);  //punto esclamativo = ha completato un'operazione
      break;
    case 2:
      print(F("Can't "), false);
      print(name, false);
      print(F(" \""), false);
      print(file, false);
      print(F("\"."), ln);  //punto = non ha completato un'operazione
      break;
    case 3:
      print(F("\""), false);
      print(file, false);
      print(F("\" "), false);  //punto = non ha completato un'operazione
      print(name, false);
      print(F("."), ln);
      break;
    case 4:
      print(name, false);
      print(F(" \""), false);
      print(file, false);
      print(F("\"?"), ln);  //punto interrogativo = chiede conferma per completare un'operazione
      break;
  }
}

void scroll() {
  int16_t cursorY = tft.getCursorY();

  tft.setTextColor(colorText);

  for (screenBufferCountY = 1; screenBufferCountY <= charY; screenBufferCountY++) {
    for (screenBufferCountX = 0; screenBufferCountX <= charX; screenBufferCountX++) {
      screenBuffer[screenBufferCountX][screenBufferCountY - 1] = screenBuffer[screenBufferCountX][screenBufferCountY];  //SHIFT BUFFER UP
    }
  }

  for (screenBufferCountX = 0; screenBufferCountX <= charX; screenBufferCountX++) screenBuffer[screenBufferCountX][charY] = ' ';  //CLEAR LAST LINE

  for (screenBufferCountY = charY; screenBufferCountY >= 0; screenBufferCountY--) {
    tft.setCursor(0, screenBufferCountY * 8);
    tft.fillRect(0, screenBufferCountY * 8, 320, 8, BLACK);  //CLEARS LINE TO WRITE
    for (screenBufferCountX = 0; screenBufferCountX < charX; screenBufferCountX++) {
      tft.print(screenBuffer[screenBufferCountX][screenBufferCountY]);  //WRITE EACH CHARACTER
    }
  }

  tft.setCursor(0, cursorY);  //RESETS CURSOR TO LAST POS
  if (bg) {
    if (!invert) tft.setTextColor(colorText, colorBack);
    else tft.setTextColor(colorBack, colorText);
  } else {
    if (!invert) tft.setTextColor(colorText);
    else tft.setTextColor(colorBack);
  }
}

char instToChar(String inst) {
  if (inst == "NOP") return 0;     //No operation
  if (inst == "LDI") return 1;     //Loads into [r1] the value of [i2]
  if (inst == "RES") return 2;     //USED ONLY FOR COMPILER! Saves as a new resource [t1]
  if (inst == "JMP_LE") return 3;  //Jumps to line [i1] if [r2] == [r3]
  if (inst == "JMP_OE") return 3;  //Jumps to offset [i1] if [r2] == [r3]
  if (inst == "JMP_LD") return 4;  //Jumps to line [i1] if [r2] != [r3]
  if (inst == "JMP_OD") return 4;  //Jumps to offset [i1] if [r2] != [r3]
  if (inst == "ADD") return 5;     //Saves in [r1] the value of [r2] + [r3]
  if (inst == "SUB") return 6;     //Saves in [r1] the value of [r2] - [r3]
  if (inst == "MUL") return 7;     //Saves in [r1] the value of [r2] * [r3]
  if (inst == "DIV") return 8;     //Saves in [r1] the value of [r2] / [r3]
  if (inst == "KEY") return 9;     //Saves in [r1] the value of the current pressed key at the time of call
  if (inst == "CLS") return 10;    //Clears screen
  if (inst == "CRS_I") return 11;  //Sets cursor to X = [i1] Y = [i2]
  if (inst == "CRS_R") return 12;  //Sets cursor to X = [r1] Y = [r2]
  if (inst == "WRT_T") return 13;  //Sets background as [b2] and writes to screen the resource saved to pos [i1]
  if (inst == "WRT_I") return 14;  //Sets background as [b2] and writes to screen [i1]
  if (inst == "WRT_R") return 15;  //Sets background as [b2] and writes to screen [r1]
  if (inst == "END") return 255;   //Stops execution of the program
  found = false;
}

void regReset(File dir, uint16_t reg) {
  reg *= regSize;
  dir.seek(reg);
  for (uint8_t i = 0; i < regSize; i++) dir.print(char(0));
  dir.seek(reg);
}

String readHexFrom16(File dir, uint16_t reg) {
  dir.seek(reg * regSize);
  uint8_t char1 = dir.read();
  uint8_t char2 = dir.read();
  return String(char1, 16) + String(char2, 16);  //Converte la prima e la seconda parte del parametro da decimale a esadecimale e le unisce
}

String readHex16(File dir) {
  uint8_t char1 = dir.read();
  uint8_t char2 = dir.read();
  return String(char1, 16) + String(char2, 16);
}

String readHex8(File dir) {
  return String(dir.read(), 16);
}

int hextol(String hex) {
  int result = strtol(hex.c_str(), NULL, 16);
  if (result > 32767) result -= 65536;  //Lo rende negativo
  return result;
}

String ltohex(int val) {
  return String(val, 16);
}