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

void scroll() {                 //Add different modes of scroll //I will not (for now)
  int16_t countX, countY;       //NEED TO BE SIGNED they are in conditions that requires negative numbers
  tft.setTextColor(colorText);  //To make sure to not use the background (its wayyy faster)

  for (countY = 1; countY <= charY; countY++) {                                                               //For each line in the buffer, starting from the second...
    for (countX = 0; countX <= charX; countX++) textBuffer[countX][countY - 1] = textBuffer[countX][countY];  //For each char in the line, shift it one line up in the buffer
  }

  for (countX = 0; countX <= charX; countX++) textBuffer[countX][charY] = ' ';  //Clears the last line of the buffer only once

  for (countY = charY; countY >= 0; countY--) {                                        //For each line on the screen...
    tft.setCursor(0, countY * 8);                                                      //Set cursor at the start of the line to write
    tft.fillRect(0, countY * 8, 320, 8, colorBack);                                    //Clears the line with the background color
    for (countX = 0; countX < charX; countX++) tft.print(textBuffer[countX][countY]);  //For each char in the buffer, write that char
  }

  tft.setCursor(0, 232);  //Sets cursor to position before scroll
}

char instToChar(String inst) {     //List of AR-ASM instructions and a small description
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
  found = false;                   //If the function does not find an instruction, sets found to false
}

void regReset(File dir, uint16_t reg) {  //Sets an [regSize] amount of bytes to NULL and jumps to previous position
  reg *= regSize;
  dir.seek(reg);
  for (uint8_t i = 0; i < regSize; i++) dir.print(char(0));
  dir.seek(reg);
}

String readHexFrom16(File dir, uint16_t reg) {  //Reads 2 hex bytes in a defined position from a file
  dir.seek(reg * regSize);
  uint8_t char1 = dir.read();
  uint8_t char2 = dir.read();
  return String(char1, 16) + String(char2, 16);  //Converts the first and second part of the parameter from decimal to hexadecimal and merges them
}

String readHex16(File dir) {  //Reads 2 hex bytes from a file
  uint8_t char1 = dir.read();
  uint8_t char2 = dir.read();
  return String(char1, 16) + String(char2, 16);
}

String readHex8(File dir) {  //Reads 1 hex byte from a file
  return String(dir.read(), 16);
}

void printHex(File dir, uint8_t i) {
  if (inputFrag[i].length() < 4) {  //Standardizza la stringa a 4 caratteri
    String temp = inputFrag[i];
    inputFrag[i] = "";
    while (inputFrag[i].length() + temp.length() <= 4) inputFrag[i] += "0";
    inputFrag[i] += temp;
  }
  if (inputFrag[i].length() > 4) inputFrag[i] = inputFrag[i].substring(inputFrag[i].length() - 4, inputFrag[i].length());

  dir.print(char(hextol(inputFrag[i].substring(0, 2))));  //Converte la prima sezione in una stringa C, converte da esadecimale a decimale, converte in un carattere e lo stampa
  dir.print(char(hextol(inputFrag[i].substring(2, 4))));  //Converte la seconda sezione in una stringa C, converte da esadecimale a decimale, converte in un carattere e lo stampa
}

int hextol(String hex) {  //Converts an hex string into a signed number
  int result = strtol(hex.c_str(), NULL, 16);
  if (result > 32767) result -= 65536;  //Lo rende negativo
  return result;
}

String ltohex(int val) {  //Converts a signed number into a hex string
  return String(val, 16);
}

File fileOpen(String file) {  //Opens a file path
  return SD.open(file.c_str());
}

File fileOpen(String file, uint8_t mode) {  //Opens a file path in a specified mode
  return SD.open(file.c_str(), mode);
}

bool dirMake(String dir) {  //Creates a dir from a path
  return SD.mkdir(dir.c_str());
}

bool dirRemove(String dir) {  //Removes a dir from a path
  return SD.rmdir(dir.c_str());
}

bool fileRemove(String file) {  //Creates a file from a path
  return SD.remove(file.c_str());
}

bool fileExists(String file) {  //Checks if a file from a path exists
  return SD.exists(file.c_str());
}

bool fileAvailable(File file) {  //Checks if a file is available
  return file.available();
}

String fileRead(File file) {  //Reads the contents of a file
  String read;
  while (file.available()) read += fileReadChar(file);
  return read;
}

String fileReadLine(File file) {  //Reads a line of file
  String read;
  char currentChar;
  while ((currentChar = file.read()) != '\n' && file.available()) read += currentChar;  //Saves all chars until a new line
  if (currentChar != '\n') read += currentChar;                                         //The last char is not saved, so it re-does the procedure again one time (idk why it works like that)
  return read;
}

char fileReadChar(File file) {  //Reads a byte (char) of file
  return char(file.read());
}

String formatPath(String path) {  //Formats a string in the form of a path readable to the SD lib
  path.replace('\\', '/');        //Replaces slashes
  path.toUpperCase();
  return path;
}