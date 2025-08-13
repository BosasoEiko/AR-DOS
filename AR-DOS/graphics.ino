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
      textBuffer[tft.getCursorX() / charWidth][tft.getCursorY() / charHeight] = data.charAt(charCount);
      tft.print(data.charAt(charCount));
    } else {
      if (tft.getCursorY() + charHeight >= tft.height()) scroll();
      else tft.println();
    }
  }

  if (ln) {
    if (tft.getCursorY() + charHeight >= tft.height()) scroll();
    else tft.println();
  }

  printed = true;
}
void printPath(String data) {
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
    tft.setCursor(0, countY * charHeight);                                                      //Set cursor at the start of the line to write
    tft.fillRect(0, countY * charHeight, tft.width(), charHeight, colorBack);                                    //Clears the line with the background color
    for (countX = 0; countX < charX; countX++) tft.print(textBuffer[countX][countY]);  //For each char in the buffer, write that char
  }
  tft.setCursor(cursorX, cursorY);
}
//FUNCTIONS
void scroll() {                 //Add different modes of scroll //I will not (for now)
  int16_t countX, countY;       //NEED TO BE SIGNED they are in conditions that requires negative numbers
  tft.setTextColor(colorText);  //To make sure to not use the background (its wayyy faster)

  for (countY = 1; countY <= charY; countY++) {                                                               //For each line in the buffer, starting from the second...
    for (countX = 0; countX <= charX; countX++) textBuffer[countX][countY - 1] = textBuffer[countX][countY];  //For each char in the line, shift it one line up in the buffer
  }

  for (countX = 0; countX <= charX; countX++) textBuffer[countX][charY] = ' ';  //Clears the last line of the buffer only once

  for (countY = charY; countY >= 0; countY--) {                                        //For each line on the screen...
    tft.setCursor(0, countY * charHeight);                                                      //Set cursor at the start of the line to write
    tft.fillRect(0, countY * charHeight, tft.width(), charHeight, colorBack);                                    //Clears the line with the background color
    for (countX = 0; countX < charX; countX++) tft.print(textBuffer[countX][countY]);  //For each char in the buffer, write that char
  }

  tft.setCursor(0, 232);  //Sets cursor to position before scroll
}
void clearScreen() {
  tft.fillScreen(colorBack);
  tft.setCursor(0, 0);
  for (uint8_t y = 0; y < charY; y++) {
    for (uint8_t x = 0; x < charX; x++) textBuffer[x][y] = ' ';
  }
}
void cursorMove(int16_t xDelta, int16_t yDelta) {
  float x = tft.getCursorX() + xDelta * charWidth, y = tft.getCursorY() + yDelta * charHeight;  //Get cursor positions from char positions
  int16_t yWarp = floor(x / tft.width());                                                       //How many times it needs to warp around on the y axis //x is already a float, so the operands are treated as floats
  tft.setCursor(x - yWarp * tft.width(), y + yWarp);
}