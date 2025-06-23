//NOTE: Put background bool as a parameter (maybe also inverted)

void print(String data, bool ln) {
  for (int charCount = 0; charCount < data.length(); charCount++) printChar(data.charAt(charCount));
  if (ln) print(true);
}

void print(char data, bool ln) {
  printChar(data);
  if (ln) print(true);
}

void print(int par1, bool ln) {
  String data = String(par1);
  for (int charCount = 0; charCount < data.length(); charCount++) printChar(data.charAt(charCount));
  if (ln) print(true);
}

void print(int par1, uint8_t base, bool ln) {
  String data = String(par1, base);
  for (int charCount = 0; charCount < data.length(); charCount++) printChar(data.charAt(charCount));
  if (ln) print(true);
}

void print(bool ln) {
  if (ln) {
    if (tft.getCursorY() + 8 >= 240) scroll();
    else tft.println();
  }
}

void printChar(char data) {
  if (bg) {
    if (!invert) tft.setTextColor(colorText, colorBack);
    else tft.setTextColor(colorBack, colorText);
  } else {
    if (!invert) tft.setTextColor(colorText);
    else tft.setTextColor(colorBack);
  }
  if (data == '\n') {
    print(true);
    return;
  } else textBuffer[tft.getCursorX() / 6][tft.getCursorY() / 8] = data;
  if (tft.getCursorY() >= 240) scroll();
  tft.print(data);
  printed = true;
}

void printDir(String data) {
  print(data.substring(0, 2), false);                                    //Prints the disk letter
  if (data.charAt(2) != '/') print('\\', false);                         //If the path is root (so there is no slash after), prints a slash (to match MS-DOS)
  for (uint8_t charCount = 2; charCount < data.length(); charCount++) {  //The first char is the disk, so it starts at 1
    char currentChar = data.charAt(charCount);
    if (currentChar == '/') print('\\', false);  //To match MS-DOS
    else print(currentChar, false);
  }
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

void printBuffer() {
  uint16_t cursorY = tft.getCursorY(), cursorX = tft.getCursorX(), textBufferCountX, textBufferCountY;
  for (textBufferCountY = 0; textBufferCountY < charY; textBufferCountY++) {                                                            //For each line on the screen...
    tft.setCursor(0, textBufferCountY * 8);                                                                                              //Set cursor at the start of the line to write
    tft.fillRect(0, textBufferCountY * 8, 320, 8, colorBack);                                                                            //Clears the line with the background color
    for (textBufferCountX = 0; textBufferCountX < charX; textBufferCountX++) tft.print(textBuffer[textBufferCountX][textBufferCountY]);  //For each char in the buffer, write that char
  }
  tft.setCursor(cursorX, cursorY);
}