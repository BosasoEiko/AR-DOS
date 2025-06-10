void edit(String par1, String par2) {
  const char* fileSel = par1.c_str();

  if (SD.exists(fileSel)) {

    error(0, F("Editing"), fileSel, true);

    File file = SD.open(fileSel, O_WRITE);
    if (!file) {
      error(2, F("edit"), fileSel, false);
      return;
    }
    file.print(par2);
    file.close();

    error(1, F("Edited"), fileSel, false);

  } else {

    error(3, F("doesn't exists"), fileSel, false);
  }
}