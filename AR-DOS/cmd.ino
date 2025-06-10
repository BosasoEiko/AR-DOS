void cmd() {
  if (loadedGui) {

    printDir(currentPath);
    print(F(">"), false);

    input(true);
    frag(3);
    inputFrag[0].toUpperCase();

    printed = false;

    if (inputFrag[0] == "CMD") cmd();
    else if (inputFrag[0] == "CMP") cmp(inputFrag[1], inputFrag[2]);
    else if (inputFrag[0] == "RUN") run(inputFrag[1]);
    else if (inputFrag[0] == "CD") cd(inputFrag[1]);
    else if (inputFrag[0] == "MAKE") make(inputFrag[1]);
    else if (inputFrag[0] == "EDIT") edit(inputFrag[1], inputFrag[2]);
    else if (inputFrag[0] == "READ") read(inputFrag[1]);
    else if (inputFrag[0] == "DEL") del(inputFrag[1]);
    else if (inputFrag[0] == "TREE") tree();
    else if (inputFrag[0] == "CLS") cls();
    else if (inputFrag[0] == "ECHO") echo(inputFrag[1]);
    else if (inputFrag[0] == "SET") set(inputFrag[1], inputFrag[2]);
    else if (inputFrag[0] == "DBG") dbg();
    else if (inputFrag[0] == "SCR") scroll();
    else if (inputFrag[0] == "EXIT") exit();
    else if (inputFrag[0] == "");
    else print(F("Command not found"), false);

    if (inputFrag[0] != "") if (printed) print('\n', true); else print(true);

  } else {
    print(F("\nAR-DOS\n"), true);
    tone(audioPin, audioHz, 500);
    loadedGui = true;
  }
}