void set(String setting, String par2) { //Not working correctly
  char currentChar;
  int currentPos;

  if (!loadedGui) {
    File settings = openFile(setFile, O_READ);
    while (settings.available()) {
      inputSaved = "";

      while ((currentChar = settings.read()) != '\n' && settings.available()) inputSaved += currentChar;  //salva il carattere alla fine di "inputSaved"
      if (currentChar != '\n') inputSaved += currentChar;                                                 //non so perche funziona, ma serve pk lultimo carattere del file non viene salvato...
      inputSaved.trim();                                                                                  //leva ogni '\n' che c'è dalla riga (spero)
      frag(parameters);                                                                                   //frammenta la riga

      if (inputFrag[0] == "audioHz") audioHz = inputFrag[1].toInt();
      if (inputFrag[0] == "CLS" && inputFrag[1] == "1") cls();
    }
    settings.close();
  } else {
    if (setting == "reset") {
      del(setFile);
      print(true);
      make(setFile);
      print(true);
      edit(setFile, "audioHz 2000\ntest testing\nCLS 1\ntest2 testing2");
      return;
    }

    File settings = openFile(setFile, FILE_WRITE);
    error(0, F("Searching"), setting, true);

    while (settings.available()) {
      inputSaved = "";

      while ((currentChar = settings.read()) != '\n' && settings.available()) inputSaved += currentChar;
      if (currentChar != '\n') inputSaved += currentChar;
      inputSaved.trim();
      frag(parameters);

      if (inputFrag[0] == setting) {

        error(1, F("Found"), setting, true);

        settings.seek(settings.position() - inputSaved.length());              //si posiziona all"inizio della riga
        for (int i = 0; i < inputSaved.length(); i++) settings.print(F(" "));  //sovrascrive la linea
        settings.seek(settings.position() - inputSaved.length());  //si posiziona all"inizio della riga
        settings.println();
        settings.print(setting);
        settings.print(F(" "));
        settings.print(par2);
        settings.close();
        return;
      }
    }
    error(3, F("doesn't exists"), setting, true);
    error(0, F("Creating"), setting, true);
    settings.println();
    settings.print(setting);
    settings.print(F(" "));
    settings.print(par2);
    settings.close();
  }
}