#include <SD.h>
#include <SPI.h>

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



void setup() {
  Serial.begin(115200);
  SD.begin(10, 11, 12, 13);

  Serial.println("\n\nStarting program!");
  uint32_t start = millis();

  //compile("/USERS/PROGRAMS/TEST.CMM", "/USERS/PROGRAMS/TEST.EXE");
  
  SD.remove("/USERS/PROGRAMS/TESTCOPY.TXT");
  File source = SD.open("/USERS/PROGRAMS/TEST.TXT"), destination = SD.open("/USERS/PROGRAMS/TESTCOPY.TXT", FILE_WRITE);
  fileReplace(source, destination, "test", "not-test", false, false);

  File copy = SD.open("/USERS/PROGRAMS/TESTCOPY.TXT");
  Serial.println('\n');
  while (copy.available()) Serial.write(copy.read());
  copy.close();

  uint32_t end = millis();
  Serial.print("\n\nCompleted in ");
  Serial.print(double(end - start) / 1000);
  Serial.print(" seconds, or ");
  Serial.print(end - start);
  Serial.print(" milliseconds.");
}

void loop() {}