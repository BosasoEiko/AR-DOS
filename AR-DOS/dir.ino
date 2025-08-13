bool dirMake(String path) {
  if (pathFull(path)) return SD.mkdir(pathFormat(path).c_str());
  else return SD.mkdir(pathSum(currentPath, path).c_str());
}

bool dirRemove(String path) {
  if (pathFull(path)) return SD.rmdir(pathFormat(path).c_str());
  else return SD.rmdir(pathSum(currentPath, path).c_str());
}

String pathFormat(String path) {  //Formats a string in the form of a path readable to the SD lib
  path.replace('\\', '/');        //Replaces slashes
  path.toUpperCase();
  for (uint8_t i = 0; i < path.length(); i++) {  //Removes extra slashes
    while (path.charAt(i) == '/' && path.charAt(i + 1) == '/') path.remove(i + 1);
  }
  return path;
}

String pathSum(String origin, String path) {
  return pathFormat(origin + "/" + path);
}

void pathChange(String path) {
  path = pathFormat(path);

  if (pathFull(path)) {                                                                 //Complete path
    if (fileOpen(path).isDirectory()) currentPath = path;                               //Changes only if it is a directory
  } else if (pathLast(path)) {                                                          //Previous folder
    while (!currentPath.endsWith("/")) currentPath.remove(currentPath.length() - 1);    //Removes the last folder
    if (currentPath != rootDir) currentPath.remove(currentPath.length() - 1);           //Also removes the slash unless its the root
  } else {                                                                              //Current path
    if (fileOpen(pathSum(currentPath, path)).isDirectory()) currentPath += "/" + path;  //Changes only if it is a directory
  }
}

bool pathFull(String path) {
  return path.charAt(1) == ':';
}

bool pathLast(String path) {
  return path.substring(0, 2) == "..";
}