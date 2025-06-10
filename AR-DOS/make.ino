void make(String par1) {
  const char* fileSel = par1.c_str();

  if (!SD.exists(fileSel)) {

    error(0, F("Making"), fileSel, true);
    File file = SD.open(fileSel, FILE_WRITE);

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