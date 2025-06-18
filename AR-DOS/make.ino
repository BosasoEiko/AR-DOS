void make(String fileSel) {
  fileSel = currentPath + "/" + fileSel;
  if (!existsFile(fileSel)) {
    error(0, F("Making"), fileSel, true);
    File file = openFile(fileSel, FILE_WRITE);
    if (!file) {
      error(2, F("make"), fileSel, false);
      return;
    }
    file.close();
    error(1, F("Made"), fileSel, false);
  } else {
    error(3, F("already exists"), fileSel, false);
  }
}