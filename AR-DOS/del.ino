void del(String par1) {
  const char* fileSel = par1.c_str();

  if (SD.exists(fileSel)) {
    error(0, F("Deleting"), fileSel, true);
    SD.remove(fileSel);
    if (SD.exists(fileSel)) {
      error(2, F("delete"), fileSel, false);
      return;
    }
    error(1, F("Deleted"), fileSel, false);
  } else {
    error(3, F("doesn't exists"), fileSel, false);
  }
}