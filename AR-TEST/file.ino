void fileReplace(String searchFilePath, String replaceFilePath, String searchString, String replaceString, bool strict) {
  uint32_t posStart = 0, posEnd = 0, searchStringLength = searchString.length();
  String searcher = "", foundString = "";

  if (searchFilePath == replaceFilePath) {
    replaceFilePath.remove((SD.open(searchFilePath.c_str())).name());
    replaceFilePath += "1.TMP";
    File searchFileTemp = SD.open(searchFilePath.c_str(), FILE_WRITE), replaceFileTemp = SD.open(replaceFilePath.c_str(), FILE_WRITE);
    fileCopy(searchFileTemp, replaceFileTemp, -1, -1);
    searchFileTemp.close();
    replaceFileTemp.close();
    SD.remove(searchFilePath.c_str());

    String temp = searchFilePath;
    searchFilePath = replaceFilePath;
    replaceFilePath = temp;
  }

  File searchFile = SD.open(searchFilePath.c_str()), replaceFile = SD.open(replaceFilePath.c_str(), FILE_WRITE);

  if (strict) searchStringLength += 2;

  while (searchFile.available()) {
    searcher += char(searchFile.read());
    Serial.print("Searcher: ");
    Serial.println(searcher);
    if (searcher.length() > searchStringLength) searcher = searcher.substring(1);
    foundString = searcher;
    if (strict) foundString.trim();

    if (foundString == searchString) {
      Serial.print("Found: ");
      Serial.println(foundString);
      posEnd = searchFile.position();
      searchFile.seek(posStart);
      fileCopy(searchFile, replaceFile, -1, posEnd - searchStringLength);
      replaceFile.print(replaceString);
      searchFile.seek(posEnd);
      searcher = "";
    }    
  }

  fileCopy(searchFile, replaceFile, -1, -1);

  searchFile.close();
  replaceFile.close();
}

void fileCopy(File& source, File& destination, uint8_t bufferSize, uint32_t until) {//Set until to -1 to copy the whole file, and bufferSize to -1 to use the whole buffer
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