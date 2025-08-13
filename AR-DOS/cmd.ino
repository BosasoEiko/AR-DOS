//WARNING! : Control is a feature in testing, and it will be used later with the addition of multitasking.
//Right now it's useless, and it only moves GRAPHICALLY the cursor around the screen, messing with the command line

void cmd() {  //Command-line based shell //In the future, it will be a EXE file
  while (1) {
    printPath(currentPath);
    print(F(">"));

    bool enter = false;
    inputSaved = '\0';
    unsigned long start = millis(), end; //Used for blinking cursor

    //A memorial to the infamous code i wrote in class that handled the alphanumeric keypad (FINALLY REPLACED BY A PS/2 KEYBOARD)
    while (!enter) {
      if (kb.available()) {
        char c = km.remapKey(kb.read());
        switch (c) {
          default:
            inputSaved += c;
            break;
          case PS2_KEY_ESC:  //Not working (used ESC key for testing because it works)
            enter = true;
            break;
        }
        print(c);
      }
    }
    println(); //For testing. Willl be replaced by the \n char when sending the command

    frag(inputSaved, inputFrag);
    inputFrag[0].toUpperCase();  //Commands are always uppercase

    printed = false;

    //It will be replaced by a file search

    if (inputFrag[0] == "CMD") cmd();
    else if (inputFrag[0] == "CD") pathChange(inputFrag[1]);
    else if (inputFrag[0] == "MAKE") fileOpen(inputFrag[1], O_CREAT);
    else if (inputFrag[0] == "MD" || inputFrag[0] == "MKDIR") dirMake(inputFrag[1]);
    else if (inputFrag[0] == "RM" || inputFrag[0] == "RMDIR") dirRemove(inputFrag[1]);
    else if (inputFrag[0] == "EDIT") fileWrite(fileOpen(inputFrag[1], O_WRITE), inputFrag[2]);
    else if (inputFrag[0] == "TYPE") print(fileRead(fileOpen(inputFrag[1])));
    else if (inputFrag[0] == "DEL") fileRemove(inputFrag[1]);
    else if (inputFrag[0] == "TREE") tree();
    else if (inputFrag[0] == "CLS") clearScreen();
    else if (inputFrag[0] == "ECHO") print(inputFrag[1]);
    else if (inputFrag[0] == "EXIT") return;
    else if (inputFrag[0] != "") print(F("Bad command or file name"));

    if (inputFrag[0] != "") {
      if (printed) println('\n');
      else println();
    }
  }
}