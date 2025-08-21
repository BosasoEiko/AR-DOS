uint32_t fileReplace(String sourcePath, String destinationPath, String search, String replace, bool strict, bool sensitive) {  //Creates a copy of a file replacing the defined string with another string, and returns how many times it encountered the string
  File source, destination;
  uint32_t start = 0, end = 0, counter = 0;
  String buffer = "";

  if (sourcePath == destinationPath) {
#ifdef DEBUG
    Serial.println("Replace mode");
#endif
    source = filePush(tempDir, tempCount);
    fileCopy(SD.open(sourcePath.c_str()), source, -1, -1);
    SD.remove(sourcePath.c_str());
    source.seek(0);
  } else source = SD.open(sourcePath.c_str());
  destination = SD.open(destinationPath.c_str(), FILE_WRITE);

  while (source.available()) {
    buffer += char(source.read());
    if (buffer.length() > search.length()) buffer = buffer.substring(1);

#ifdef DEBUG
    Serial.print("Searching...\tPrevious: ");
    Serial.write(filePeek(source, source.position() - buffer.length() - 1));
    Serial.print(";\tBuffer: ");
    Serial.print(buffer);
    Serial.print(";\tNext: ");
    Serial.write(filePeek(source, source.position()));
    Serial.print(";\tStart: ");
    Serial.print(start);
    Serial.print(";\tEnd: ");
    Serial.print(end);
    Serial.println(";");
#endif

    if (((sensitive && buffer.equals(search)) || (!sensitive && buffer.equalsIgnoreCase(search))) && ((strict && !isChar(filePeek(source, source.position() - buffer.length() - 1)) && !isChar(filePeek(source, source.position()))) || !strict)) {  //help

#ifdef DEBUG
      Serial.print("Found: ");
      Serial.println(buffer);
#endif
      counter++;
      end = source.position();                                                          //Saves the search position
      source.seek(start);                                                               //Seeks to the start of the search
      if (source.position()) fileCopy(source, destination, -1, end - buffer.length());  //If it's NOT at the start of the file, copies the contents from the start of the search to right before the string that needs to be replaced
      destination.print(replace);                                                       //Prints the replace string
      source.seek(end);                                                                 //Returns to the previous search position
      start = end;                                                                      //Saves the start of the new search
      buffer = "";
    }
  }

#ifdef DEBUG
  Serial.println("Replaced!\nDestination contents:");
  destination.seek(0);
  while (destination.available()) Serial.write(destination.read());
  Serial.println();
#endif

  if (sourcePath == destinationPath) filePop(tempDir, tempCount);
  else source.close();
  destination.close();
  return counter;
}

void fileCopy(File source, File destination, uint8_t bufferSize, uint32_t until) {  //Set until to -1 to copy the whole file, and bufferSize to -1 to use the whole buffer

#ifdef DEBUG
  Serial.print("Copying...\tFrom: ");
  Serial.print(source.position());
  Serial.print(";\tUntil: ");
  Serial.print(until);
  Serial.println(";");
#endif

  String buffer = "";
  while (source.available() && source.position() < until) {

#ifdef DEBUG
    Serial.println("Buffering...");
#endif

    while (buffer.length() <= bufferSize && source.available() && source.position() < until) buffer += char(source.read());

#ifdef DEBUG
    Serial.print("Buffered: ");
    Serial.println(buffer);
#endif

    destination.print(buffer);
    buffer = "";
  }

#ifdef DEBUG
  Serial.println("Copied!");
#endif
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

File filePush(String &path, uint32_t &counter) {
  counter++;

#ifdef DEBUG
  Serial.print("New file created: " + path + i2hex(tempCount, 4) + ".TMP\t ");
  Serial.println(counter);
#endif

  return SD.open((path + i2hex(tempCount, 4) + ".TMP").c_str(), FILE_WRITE);
}

void filePop(String &path, uint32_t &counter) {
  SD.remove((path + i2hex(tempCount, 4) + ".TMP").c_str());

#ifdef DEBUG
  Serial.print("Old file removed: " + path + i2hex(tempCount, 4) + ".TMP\t ");
  Serial.println(counter);
#endif

  if (counter) counter--;
}