void md(String dir) {
  dir = formatPath(dir);

  if (dir.charAt(1) == ':') {  //Complete path
    if (!dirMake(dir)) print(F("Invalid path"));
  } else {  //Current path
    if (!dirMake(currentPath + "/" + dir)) print(F("Invalid path"));
  }
}