void make(String fileSel) {
  fileSel = formatPath(fileSel);
  if (fileSel.charAt(1) == ':') fileOpen(fileSel, O_CREAT).close();  //Complete path
  else fileOpen(currentPath + "/" + fileSel, O_CREAT).close();       //Current path
  return fileExists(fileSel);
}