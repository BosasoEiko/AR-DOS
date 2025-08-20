void fileReplace(File searchFile, File replaceFile, String searchString, String replaceString, bool strict, bool sensitive) {
  uint32_t posStart = 0, posEnd = 0;
  String foundString = "";

  while (searchFile.available()) {
    foundString += char(searchFile.read());
    if (foundString.length() > searchString.length()) foundString = foundString.substring(1);
    Serial.print("Previous: ");
    Serial.write(filePeek(searchFile, searchFile.position() - foundString.length() - 1));
    Serial.print("; Searching: ");
    Serial.print(foundString);
    Serial.print("; Next: ");
    Serial.write(filePeek(searchFile, searchFile.position()));
    Serial.println(";");


    if (((sensitive && foundString.equals(searchString)) || (!sensitive && foundString.equalsIgnoreCase(searchString))) && ((strict && !isChar(filePeek(searchFile, searchFile.position() - foundString.length() - 1)) && !isChar(filePeek(searchFile, searchFile.position()))) || !strict)) {  //help
      Serial.print("FOUND: ");
      Serial.println(foundString);
      posEnd = searchFile.position();
      searchFile.seek(posStart);
      fileCopy(searchFile, replaceFile, -1, posEnd - searchString.length());
      replaceFile.print(replaceString);
      searchFile.seek(posEnd);
      foundString = "";
    }
  }

  fileCopy(searchFile, replaceFile, -1, -1);
}

void fileCopy(File source, File destination, uint8_t bufferSize, uint32_t until) {  //Set until to -1 to copy the whole file, and bufferSize to -1 to use the whole buffer
  Serial.println("Copying...  ");
  Serial.println(source.position());
  Serial.println(destination.position());
  String buffer = "";
  while (source.available() && source.position() < until) {
    if (buffer.length() <= bufferSize && source.available() && source.position() < until) buffer += char(source.read());
    else {
      destination.print(buffer);
      Serial.println(buffer);
      buffer = "";
    }
  }
  Serial.println("Finished copying!");
}

int16_t filePeek(File file, uint32_t pos) {
  if (pos < file.size()) {
    uint32_t currentPos = file.position();
    file.seek(pos);
    int16_t read = file.read();
    file.seek(currentPos);
    return read;
  } else return -1;
}