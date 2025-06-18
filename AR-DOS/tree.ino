int fileFound, dirFound;

void tree() {
  fileFound = 0;
  dirFound = 0;
  printDirectory(openFile(rootDir), 0);
  print(F("\nDisplayed "), false);
  print(dirFound, false);
  print(F(" dir(s) and "), false);
  print(fileFound, false);
  print(F(" file(s)"), false);
}

void printDirectory(File dir, uint8_t numTabs) {
  dir.rewindDirectory();
  printDir(dir.name()); //test
  print(true);
  while (1) {
    File entry = dir.openNextFile();
    if (!entry) break;
    for (uint8_t i = 0; i < numTabs; i++) print(F(" "), false);
    print(entry.name(), false);
    if (entry.isDirectory()) {
      dirFound++;
      print(F("\\"), true);
      printDirectory(entry,  + 1);
    } else {
      fileFound++;
      print(F("  "), false);
      print(entry.size(), 10, true);
    }
    entry.close();
  }
}