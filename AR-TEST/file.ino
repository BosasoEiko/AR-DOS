void fileReplace(File source, File destination, String search, String replace, bool strict, bool sensitive) {
  int16_t c;
  uint32_t start = 0, end = 0;
  String found = "";

  while (source.available()) {
    found += char(source.read());
    if (found.length() > search.length()) found = found.substring(1);
    Serial.print("Searching...\tPrevious: ");
    Serial.write(filePeek(source, source.position() - found.length() - 1));
    Serial.print(";\tSearching: ");
    Serial.print(found);
    Serial.print(";\tNext: ");
    Serial.write(filePeek(source, source.position()));
    Serial.print(";\tStart: ");
    Serial.print(start);
    Serial.print(";\tEnd: ");
    Serial.print(end);
    Serial.println(";");

    if (((sensitive && found.equals(search)) || (!sensitive && found.equalsIgnoreCase(search))) && ((strict && !isChar(filePeek(source, source.position() - found.length() - 1)) && !isChar(filePeek(source, source.position()))) || !strict)) {  //help
      Serial.print("-=[ FOUND ]=- -=[");
      Serial.print(found);
      Serial.println("]=-");
      end = source.position();
      source.seek(start);
      if (source.position()) { //If it's at the start of the file, don't copy
        fileCopy(source, destination, -1, end - found.length() - 1);
        destination.write(filePeek(source, source.position()));
      }
      destination.print(replace);
      source.seek(end);
      start = end;
      found = "";
    }
  }
}

void fileCopy(File source, File destination, uint8_t bufferSize, uint32_t until) {  //Set until to -1 to copy the whole file, and bufferSize to -1 to use the whole buffer
  Serial.print("Copying...\tFrom: ");
  Serial.print(source.position());
  Serial.print(";\tUntil: ");
  Serial.print(until);
  Serial.println(";");

  String buffer = "";
  while (source.available() && source.position() < until) {
    Serial.println("Buffering...");
    while (buffer.length() <= bufferSize && source.available() && source.position() < until) buffer += char(source.read());
    Serial.print("Buffered: ");
    Serial.println(buffer);
    destination.print(buffer);
    buffer = "";
  }
  Serial.println("Copied!");
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