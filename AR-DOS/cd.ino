void cd(String dir) {
  File file;
  dir.replace('\\', '/');  //Replaces slashes
  switch (dir.charAt(0)) { //Checks what action sould it do
    default:
      file = openFile(currentPath + "/" + dir);
      if (file.isDirectory()) {
        currentPath += "/" + String(file.name());
      }
      break;
    case '/':
      file = openFile(dir);
      if (file.isDirectory()) {
        currentPath = dir;
      }
      break;
    case '.':
      while (!currentPath.endsWith("/")) currentPath.remove(currentPath.length() - 1);  //Removes the last folder
      if (currentPath != rootDir) currentPath.remove(currentPath.length() - 1);         //Removes also the slash, only when it's not the root
      break;
  }
  file.close();
}