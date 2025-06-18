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
  } else screenBuffer[tft.getCursorX() / 6][tft.getCursorY() / 8] = data;
  if (tft.getCursorY() >= 240) scroll();
  tft.print(data);
  printed = true;
}

void printDir(String data) {
  for (int charCount = 0; charCount < data.length(); charCount++) {
    char currentChar = data.charAt(charCount);
    if (currentChar == '/') print('\\', false);
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