void read(String fileSel) {
  if (existsFile(fileSel)) {
    error(0, F("Reading"), fileSel, true);
    File file = openFile(fileSel, O_READ);

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