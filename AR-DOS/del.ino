void del(String fileSel) {
  fileSel = formatPath(fileSel);
  if (fileSel.charAt(1) == ':') fileRemove(fileSel);  //Complete path
  else fileRemove(currentPath + "/" + fileSel);       //Current path
}