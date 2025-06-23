void type(String fileSel) {
  fileSel = formatPath(fileSel);
  File file;
  if (fileSel.charAt(1) == ':') file = fileOpen(fileSel, O_READ);  //Complete path
  else file = fileOpen(currentPath + "/" + fileSel, O_READ);       //Current path
  while (file.available()) print(fileReadChar(file), false);        //Prints the bytes as chars
  file.close();
}