void edit(String fileSel, String par2) {
  fileSel = formatPath(fileSel);
  File file;
  if (fileSel.charAt(1) == ':') file = fileOpen(fileSel, O_WRITE);  //Complete path
  else file = fileOpen(currentPath + "/" + fileSel, O_WRITE);       //Current path
  file.print(par2);
  file.close();
}