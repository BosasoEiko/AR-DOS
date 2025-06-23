void set(String setting, String par2) {
  if (setting == "load") {                      //Loads settings
    File settings = fileOpen(setFile, FILE_WRITE);  //Opens settings file. Creates the file if not existant
    while (fileAvailable(settings)) {           //Reads all content of file

      inputSaved = fileReadLine(settings);  //Reads line by line (or setting by setting)
      frag(parameters);                     //Fragments the saved string in parameters

      if (inputFrag[0] == "speakerHz") speakerHz = inputFrag[1].toInt();
      else if (inputFrag[0] == "speakerPin") speakerPin = inputFrag[1].toInt();
      else if (inputFrag[0] == "clearBoot") cls();
      else if (inputFrag[0] == "screenRefresh") par2 = "1"; //Enables the screen refresh parameter (NEEDS TO BE LAST)
    }

    settings.close();

    if (par2 == "1") printBuffer(); //Screen refresh
  }

  /* else { Does not work currently, might as well not compile it
    if (setting == "reset") {
      del(setFile);
      print(true);
      make(setFile);
      print(true);
      edit(setFile, "speakerHz 2000\ntest testing\nCLS 1\ntest2 testing2");
      return;
    }

    File settings = fileOpen(setFile, FILE_WRITE);
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
  } */
}