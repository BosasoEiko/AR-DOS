void frag(uint8_t par) {
  int charCount = 0;
  for (uint8_t parCurrent = 0; parCurrent < par; parCurrent++) inputFrag[parCurrent] = "";
  for (uint8_t parCurrent = 0; parCurrent < par; parCurrent++) {
    bool withSpace = false;
    if (parCurrent != 0) charCount++;  //Skips the space
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
      print(F("\"..."), ln);  //... = Executing an operation
      break;
    case 1:
      print(name, false);
      print(F(" \""), false);
      print(file, false);
      print(F("\"!"), ln);  //! = Completed an operation
      break;
    case 2:
      print(F("Can't "), false);
      print(name, false);
      print(F(" \""), false);
      print(file, false);
      print(F("\"."), ln);  //. = Hasn't completed an operation
      break;
    case 3:
      print(F("\""), false);
      print(file, false);
      print(F("\" "), false);  //. = Hasn't completed an operation
      print(name, false);
      print(F("."), ln);
      break;
    case 4:
      print(name, false);
      print(F(" \""), false);
      print(file, false);
      print(F("\"?"), ln);  //? = Need confirmation for an operation
      break;
  }
}

void scroll() {  //Need to fix a lot of bugs //Add different modes of scroll
  uint16_t textBufferCountX, textBufferCountY;
  tft.setTextColor(colorText);  //No background

  for (textBufferCountY = 1; textBufferCountY <= charY; textBufferCountY++) {                                                                                                                   //For each line in the buffer...
    for (textBufferCountX = 0; textBufferCountX <= charX; textBufferCountX++) textBuffer[textBufferCountX][textBufferCountY - 1] = textBuffer[textBufferCountX][textBufferCountY];  //For each char in the line, shift it one line up in the buffer
  }

  for (textBufferCountX = 0; textBufferCountX <= charX; textBufferCountX++) textBuffer[textBufferCountX][charY] = ' ';  //Clears the last line of the buffer

  for (textBufferCountY = charY; textBufferCountY >= 0; textBufferCountY--) {                                                                  //For each line on the screen...
    tft.setCursor(0, textBufferCountY * 8);                                                                                                        //Set cursor at the start of the line to write
    tft.fillRect(0, textBufferCountY * 8, 320, 8, colorBack);                                                                                      //Clears the line with the background color
    for (textBufferCountX = 0; textBufferCountX < charX; textBufferCountX++) tft.print(textBuffer[textBufferCountX][textBufferCountY]);  //For each char in the buffer, write that char
  }

  tft.setCursor(0, 231);  //Reset cursor to bottom of the screen
  if (bg) {               //Resets previous color settings
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
  return String(char1, 16) + String(char2, 16);  //Converts the first and second part of the parameter from decimal to hexadecimal and merges them
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

File fileOpen(String file) {
  return SD.open(file.c_str());
}

File fileOpen(String file, uint8_t mode) {
  return SD.open(file.c_str(), mode);
}

bool dirMake(String dir) {
  return SD.mkdir(dir.c_str());
}

bool dirRemove(String dir) {
  return SD.rmdir(dir.c_str());
}

bool fileRemove(String file) {
  return SD.remove(file.c_str());
}

bool fileExists(String file) {
  return SD.exists(file.c_str());
}

bool fileAvailable(File file) {
  return file.available();
}

String fileRead(File file) {
  String read;
  while (file.available()) read += fileReadChar(file);
  return read;
}

String fileReadLine(File file) {
  String read;
  char currentChar;
  while ((currentChar = file.read()) != '\n' && file.available()) read += currentChar;  //Saves all chars until a new line
  if (currentChar != '\n') read += currentChar;                                         //The last char is not saved, so it re-does the procedure again one time (idk why it works like that)
  return read;
}

char fileReadChar(File file) {
  return char(file.read());
}

String formatPath(String path) {
  path.replace('\\', '/');  //Replaces slashes
  path.toUpperCase();
  return path;
}