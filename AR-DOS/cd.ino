void cd(String dir) {
  dir = formatPath(dir);

  if (dir.charAt(1) == ':') {  //Complete path
    if (fileOpen(dir).isDirectory()) currentPath = dir;
  } else if (dir.substring(0, 2) == "..") {                                           //Previous folder
    while (!currentPath.endsWith("/")) currentPath.remove(currentPath.length() - 1);  //Removes the last folder unless it is the root
    if (currentPath != rootDir) currentPath.remove(currentPath.length() - 1);         //Also removes the slash unless its the root
  } else {                                                                            //Current path
    if (fileOpen(currentPath + "/" + dir).isDirectory()) currentPath += "/" + dir;
  }
}