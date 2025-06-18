void edit(String fileSel, String par2) {

  if (existsFile(fileSel)) {

    error(0, F("Editing"), fileSel, true);

    File file = openFile(fileSel, O_WRITE);
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