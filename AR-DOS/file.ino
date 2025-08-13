File fileOpen(String path) {
  if (pathFull(path)) return SD.open(pathFormat(path).c_str());
  else return SD.open(pathSum(currentPath, path).c_str());
}

File fileOpen(String path, uint16_t mode) {
  if (pathFull(path)) return SD.open(pathFormat(path).c_str(), mode);
  else return SD.open(pathSum(currentPath, path).c_str(), mode);
}

bool fileRemove(String path) {
  if (pathFull(path)) return SD.remove(pathFormat(path).c_str());
  else return SD.remove(pathSum(currentPath, path).c_str());
}

bool fileExists(String path) {
  if (pathFull(path)) return SD.exists(pathFormat(path).c_str());
  else return SD.exists(pathSum(currentPath, path).c_str());
}

int fileAvailable(File file) {
  return file.available();
}

uint32_t filePos(File file) {
  return file.position();
}

bool fileSeek(File file, uint32_t pos) {
  return file.seek(pos);
}

String fileRead(File file) {  //Reads the contents of a file
  String read;
  while (fileAvailable(file)) read += fileReadChar(file);
  return read;
}

String fileReadLine(File file) {  //Reads a line of file
  String read;
  char currentChar;
  while ((currentChar = fileReadChar(file)) != '\n' && fileAvailable(file)) read += currentChar;  //Saves all chars until a new line
  if (currentChar != '\n') read += currentChar;                                                   //The last char is not saved, so it re-does the procedure again one time (idk why it works like that)
  return read;
}

char fileReadChar(File file) {  //Reads a byte (char) of file
  return char(file.read());
}

uint16_t fileWrite(File file, String data) {
  return file.print(data);
}