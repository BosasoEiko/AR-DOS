void read(String par1) {
  const char* fileSel = par1.c_str();

  if (SD.exists(fileSel)) {
    error(0, F("Reading"), fileSel, true);
    File file = SD.open(fileSel, O_READ);

    if (!file) {
      error(2, F("read"), fileSel, false);
      return;
    }

    print(true);
    while (file.available()) print(char(file.read()), false);
    file.close();
    print(F("\n"), true);

    error(1, F("Read"), fileSel, false);

  } else {

    error(3, F("doesn't exists"), fileSel, false);
  }
}