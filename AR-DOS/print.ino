//ln = Line New, bg = BackGround, in = Inverted
//STRING ----------------------------------------------------------------
void print(String data) {
  printHelper(data, false, false, false);
}
void println(String data) {
  printHelper(data, true, false, false);
}
void print(String data, bool bg, bool in) {
  printHelper(data, false, bg, in);
}
void println(String data, bool bg, bool in) {
  printHelper(data, true, bg, in);
}
//CHAR ----------------------------------------------------------------
void print(char data) {
  printHelper(String(data), false, false, false);
}
void println(char data) {
  printHelper(String(data), true, false, false);
}
void print(char data, bool bg, bool in) {
  printHelper(String(data), false, bg, in);
}
void println(char data, bool bg, bool in) {
  printHelper(String(data), true, bg, in);
}
//INT ----------------------------------------------------------------
void print(int data) {
  printHelper(String(data), false, false, false);
}
void println(int data) {
  printHelper(String(data), true, false, false);
}
void print(int data, bool bg, bool in) {
  printHelper(String(data), false, bg, in);
}
void println(int data, bool bg, bool in) {
  printHelper(String(data), true, bg, in);
}
//INT BASE ----------------------------------------------------------------
void print(int32_t data, uint8_t base) {
  printHelper(String(data, base), false, false, false);
}
void println(int32_t data, uint8_t base) {
  printHelper(String(data, base), true, false, false);
}
void print(int32_t data, uint8_t base, bool bg, bool in) {
  printHelper(String(data, base), false, bg, in);
}
void println(int32_t data, uint8_t base, bool bg, bool in) {
  printHelper(String(data, base), true, bg, in);
}
//LN ----------------------------------------------------------------
void println() {
  printHelper("", true, false, false);
}
//HELPERS ----------------------------------------------------------------
void printHelper(String data, bool ln, bool bg, bool in) {
  for (uint16_t charCount = 0; charCount < data.length(); charCount++) {

    if (!bg) {                                          //bg = 0
      if (!in) tft.setTextColor(colorText);             //in = 0
      else tft.setTextColor(colorBack);                 //in = 1
    } else {                                            //bg = 1
      if (!in) tft.setTextColor(colorText, colorBack);  //in = 0
      else tft.setTextColor(colorBack, colorText);      //in = 1
    }

    if (data.charAt(charCount) != '\n') {
      textBuffer[tft.getCursorX() / 6][tft.getCursorY() / 8] = data.charAt(charCount);
      tft.print(data.charAt(charCount));
    } else {
      if (tft.getCursorY() + 8 >= 240) scroll();
      else tft.println();
    }
  }

  if (ln) {
    if (tft.getCursorY() + 8 >= 240) scroll();
    else tft.println();
  }

  printed = true;
}

void printDir(String data) {
  print(data.substring(0, 2));                                           //Prints the disk letter
  if (data.charAt(2) != '/') print('\\');                                //If the path is root (so there is no slash after), prints a slash (to match MS-DOS)
  for (uint8_t charCount = 2; charCount < data.length(); charCount++) {  //The first char is the disk, so it starts at 1
    char currentChar = data.charAt(charCount);
    if (currentChar == '/') print('\\');  //To match MS-DOS
    else print(currentChar);
  }
}

void printBuffer() {
  uint16_t cursorY = tft.getCursorY(), cursorX = tft.getCursorX(), countX, countY;
  for (countY = 0; countY < charY; countY++) {                                         //For each line on the screen...
    tft.setCursor(0, countY * 8);                                                      //Set cursor at the start of the line to write
    tft.fillRect(0, countY * 8, 320, 8, colorBack);                                    //Clears the line with the background color
    for (countX = 0; countX < charX; countX++) tft.print(textBuffer[countX][countY]);  //For each char in the buffer, write that char
  }
  tft.setCursor(cursorX, cursorY);
}