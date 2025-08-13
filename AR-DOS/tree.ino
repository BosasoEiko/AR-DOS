//IT WILL BE A EXE FILE

int fileFound, dirFound;

void tree() {
  fileFound = 0;
  dirFound = 0;
  printDirectory(fileOpen(currentPath), 0);
  print(F("\nDisplayed "));
  print(dirFound);
  print(F(" dir(s) and "));
  print(fileFound);
  print(F(" file(s)"));
}

void printDirectory(File dir, uint8_t numTabs) {
  dir.rewindDirectory();
  while (1) {
    File entry = dir.openNextFile();
    if (!entry) break;
    for (uint8_t i = 0; i < numTabs; i++) print(F("  "));
    print(entry.name());
    if (entry.isDirectory()) {
      dirFound++;
      println(F("\\"));
      printDirectory(entry, numTabs + 1);
    } else {
      fileFound++;
      print(F("  "));
      println(entry.size(), 10);
    }
    entry.close();
  }
}