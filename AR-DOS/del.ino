void del(String fileSel) {
  fileSel = currentPath + "/" + fileSel;
  if (existsFile(fileSel)) {
    error(0, F("Deleting"), fileSel, true);
    removeFile(fileSel);
    if (existsFile(fileSel)) {
      error(2, F("delete"), fileSel, false);
      return;
    }
    error(1, F("Deleted"), fileSel, false);
  } else {
    error(3, F("doesn't exists"), fileSel, false);
  }
}