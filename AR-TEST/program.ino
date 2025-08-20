void compile(String sourcePath, String exePath) {
  String tempPath = exePath;
  tempPath.replace(".EXE", ".TMP");
  SD.remove(exePath.c_str());
  SD.remove(tempPath.c_str());
  File source = SD.open(sourcePath.c_str()), exe = SD.open(exePath.c_str(), FILE_WRITE), temp = SD.open(tempPath.c_str(), FILE_WRITE);

  bool comment = false, string = false, code = false;
  char currentChar, lastChar;
  uint32_t currentFunction, countFunction, countVariables;
  String currentLine = "";

  while (source.available()) {
    while (source.available() && (currentChar = source.read()) != '\n') {

      if (!comment && code && string) {  //Special keys check
        switch (currentChar) {
          case 'n':
            currentChar = '\n';
            break;
          case 't':
            currentChar = '\t';
            break;
          case '0':
            currentChar = '\0';
            break;
        }
        code = false;
      } else {  //Comments or string check
        switch (currentChar) {
          case '#':
            comment = true;
            break;
          case '"':
            string = !string;
            break;
          case '\\':
            code = true;
            break;
        }
      }

      if (!comment && !code && (string || !(currentChar == lastChar == ' ' || currentChar == '\t'))) currentLine += currentChar;  //If it's not a string, remove repeating spaces and tabs; if it's a string, do not remove the special chars; If it's a comment, nevermind
    }

    currentLine.trim();
    if (currentLine.length() > 0) currentLine += ' ';
    temp.print(currentLine);
    Serial.print(currentLine);

    currentLine = "";
    comment = false, string = false;
  }

  temp.seek(0);
  Serial.println('\n');
  while (temp.available()) Serial.print(char(temp.read()));

  source.close();
  temp.close();
  exe.close();
}