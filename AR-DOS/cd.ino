void cd(String dirSelPar) {
  File entry;
  String dirSel;
  dirSelPar.toUpperCase();
  
  switch (dirSelPar.charAt(1)) {

    default:
      if (currentDir.name() == "/") currentPath = String(currentDir.name()) + dirSelPar;
      else currentPath = String(currentDir.name()) + String('/') + dirSelPar;
      entry = SD.open(dirSel.c_str());
      if (entry.isDirectory()) {
        currentDir = entry;
        currentPath += currentDir.name();
      } else {
        print('\"', false);
        print(diskSel, false);
        print(':', false);
        printDir(dirSelPar);
        print(F("\" is not a valid directory."), false);
      }
      break;

    case ':':
      dirSel = dirSelPar.substring(2);
      entry = SD.open(dirSel.c_str());
      if (entry.isDirectory()) {
        currentDir = entry;
        currentPath += currentDir.name();
      } else {
        print('\"', false);
        printDir(dirSelPar);
        print(F("\" is not a valid directory."), false);
      }
      break;
      /* DA FARE ANCORA
    case '.':
      break;
    */
  }
}