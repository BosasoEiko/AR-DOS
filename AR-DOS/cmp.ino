void cmp(String name, String extension) {
  File fileSource = openFile(String(name + ".ASM"), O_READ);  //Legge il file ASSEMBLY

  String fileSelPar = name + "." + extension;                //Prende il nome del file assembly, gli leva l'estensione e la rimpiazza con quella definita
  removeFile(fileSelPar);                           //Se esistente, elimina il file che sta per scrivere
  File fileSel = openFile(fileSelPar, FILE_WRITE);  //Crea il file in modalita WRITE

  int8_t jumpOffset = 0;
  uint16_t line = 0;
  char currentChar, currentInst;

  for (uint8_t i = 0; i < parameters * 2 - 1; i++) fileSel.print(char(0));  //Scrive una riga dove ci saranno poi i dati di offset e di grandezza: [TIPO DI ESEGUIBILE] [OFFSET]

  while (fileSource.available()) {  //Controlla il programma
    inputSaved = "";
    while ((currentChar = fileSource.read()) != '\n' && fileSource.available()) inputSaved += currentChar;  //Legge riga per riga il file ASSEMBLY
    if (currentChar != '\n') inputSaved += currentChar;
    line++;
    inputSaved.trim();
    frag(parameters);

    found = true;
    currentInst = instToChar(inputFrag[0]);  //Va messa qui perche cambia "found" a seconda se ha trovato l'istruzione

    if (found && inputFrag[0] != "RES") {
      fileSel.print(currentInst);  //Stampa l'istruzione

      if (inputFrag[0].substring(0, 5) == "JMP_L") inputFrag[1] = String(inputFrag[1].toInt() - jumpOffset);              //Aggiunge l'offset al [1] del Jump to line
      else if (inputFrag[0].substring(0, 5) == "JMP_O") inputFrag[1] = String(line + inputFrag[1].toInt() - jumpOffset);  //Trasforma il JMP_O  in una linea adattata all'offset

      for (uint8_t i = 1; i < parameters; i++) {          //Scrive tutti i parametri che ha letto
        inputFrag[i] = String(inputFrag[i].toInt(), 16);  //Trasforma in esadecimale per renderlo 4 caratteri
        printHex(fileSel, i);
      }
    } else jumpOffset++;  //Se non è riconosciuta l'istruzione o è "RES", la salta e incrementa l'offset
  }

  fileSel.print(char(255));  //Fine dell'esecuzione

  uint16_t resourceOffset = fileSel.position();
  fileSource.seek(0);  //ritorna all'inizio, per fare il controllo delle istruzioni

  while (fileSource.available()) {  //Controlla le risorse (Solo Stringhe per ora)
    inputSaved = "";
    while ((currentChar = fileSource.read()) != '\n' && fileSource.available()) inputSaved += currentChar;  //Legge riga per riga il file ASSEMBLY
    if (currentChar != '\n') inputSaved += currentChar;
    inputSaved.trim();
    frag(parameters);
    found = true;
    currentInst = instToChar(inputFrag[0]);  //Va messa qui perche cambia "found" a seconda se ha trovato l'istruzione

    if (found && inputFrag[0] == "RES") {                    //Controlla che sia un istruzione vera e di caricamento risorse
      for (uint8_t i = 1; i < parameters; i++) {             //Controlla tutti i parametri
        if (inputFrag[i] != String(inputFrag[i].toInt())) {  //Contolla che non sia un intero
          if (inputFrag[i].substring(0, 2) != "//") {        //Controlla che non sia un commento

            inputFrag[0] = String(inputFrag[i].length(), 16);  //Salva la lunghezza della stinga
            printHex(fileSel, 0);                              //Scrive la lunghezza della stringa

            inputFrag[i].replace('\\', '\n');
            fileSel.print(inputFrag[i]);
            
            /* Replaced by the two lines above (maybe)
            for (uint16_t j = 0; j < inputFrag[i].length(); j++) {
              switch (char currentChar = inputFrag[i].charAt(j)) {
                default:
                  fileSel.print(currentChar);
                  break;
                case '\\':
                  fileSel.print('\n');
                  break;
              }
            }
            */

            break;  //ne ha trovato 1, lascia stare il resto (un istruzione puo utilizzare una risorsa alla volta)

          } else break;
        }
      }
    }
  }

  inputFrag[0] = String(resourceOffset, 16);  //Scrive l'offset in esadecimale
  fileSel.seek(1);                            //va alla posizione dove viene salvato l'offset
  printHex(fileSel, 0);                       //Salva l'offset

  fileSel.close();
  fileSource.close();
}