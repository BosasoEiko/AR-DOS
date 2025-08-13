/*  IT WILL BE REPLACED BY AN HIGH-LEVEL LANGUAGE, TO MAKE POSSIBLE CODING PROGRAMS WITH MULTITASKING

void compile(File file) {
  File fileSource = fileOpen(name + ".ASM", FILE_READ);  //Legge il file ASSEMBLY

  String fileSelPar = name + "." + extension;                //Prende il nome del file assembly, gli leva l'estensione e la rimpiazza con quella definita
  fileRemove(fileSelPar);                           //Se esistente, elimina il file che sta per scrivere
  File fileSel = fileOpen(fileSelPar);  //Crea il file in modalita WRITE

  int8_t jumpOffset = 0;
  uint16_t line = 0;
  char currentChar, currentInst;

  for (uint8_t i = 0; i < parLimit * 2 - 1; i++) fileSel.print(char(0));  //Scrive una riga dove ci saranno poi i dati di offset e di grandezza: [TIPO DI ESEGUIBILE] [OFFSET]

  while (fileSource.available()) {  //Controlla il programma
    inputSaved = "";
    while ((currentChar = fileSource.read()) != '\n' && fileSource.available()) inputSaved += currentChar;  //Legge riga per riga il file ASSEMBLY
    if (currentChar != '\n') inputSaved += currentChar;
    line++;
    inputSaved.trim();
    frag(parLimit);

    found = true;
    currentInst = instToChar(inputFrag[0]);  //Va messa qui perche cambia "found" a seconda se ha trovato l'istruzione

    if (found && inputFrag[0] != "RES") {
      fileSel.print(currentInst);  //Stampa l'istruzione

      if (inputFrag[0].substring(0, 5) == "JMP_L") inputFrag[1] = String(inputFrag[1].toInt() - jumpOffset);              //Aggiunge l'offset al [1] del Jump to line
      else if (inputFrag[0].substring(0, 5) == "JMP_O") inputFrag[1] = String(line + inputFrag[1].toInt() - jumpOffset);  //Trasforma il JMP_O  in una linea adattata all'offset

      for (uint8_t i = 1; i < parLimit; i++) {          //Scrive tutti i parametri che ha letto
        inputFrag[i] = String(inputFrag[i].toInt(), 16);  //Trasforma in esadecimale per renderlo 4 caratteri
        fileWriteHex(fileSel, i);
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
    frag(parLimit);
    found = true;
    currentInst = instToChar(inputFrag[0]);  //Va messa qui perche cambia "found" a seconda se ha trovato l'istruzione

    if (found && inputFrag[0] == "RES") {                    //Controlla che sia un istruzione vera e di caricamento risorse
      for (uint8_t i = 1; i < parLimit; i++) {             //Controlla tutti i parametri
        if (inputFrag[i] != String(inputFrag[i].toInt())) {  //Contolla che non sia un intero
          if (inputFrag[i].substring(0, 2) != "//") {        //Controlla che non sia un commento

            inputFrag[0] = String(inputFrag[i].length(), 16);  //Salva la lunghezza della stinga
            fileWriteHex(fileSel, 0);                              //Scrive la lunghezza della stringa

            inputFrag[i].replace('\\', '\n');
            fileSel.print(inputFrag[i]);
            
            break;  //ne ha trovato 1, lascia stare il resto (un istruzione puo utilizzare una risorsa alla volta)

          } else break;
        }
      }
    }
  }

  inputFrag[0] = String(resourceOffset, 16);  //Scrive l'offset in esadecimale
  fileSel.seek(1);                            //va alla posizione dove viene salvato l'offset
  fileWriteHex(fileSel, 0);                       //Salva l'offset

  fileSel.close();
  fileSource.close();
}

void execute(String filePar) {
  File file = fileRead(filePar);
  File ram = fileOpen(ramFile);

  for (uint16_t i = 0; i < 16384; i++) ram.print(char(0)); //Resetta la ram
  ram.seek(0);
  file.seek(1);

  uint8_t char1, char2;
  uint16_t progPos;

  inputFrag[1] = fileReadHex16(file);
  uint16_t resourceOffset = hextol(inputFrag[1]);

  file.seek(7);  //Vai alla linea dove inizia il programma

  bool executing = true;

  while (file.available() && executing) {
    inputFrag[0] = fileReadHex8(file);
    for (uint8_t i = 1; i < parLimit && file.available(); i++) inputFrag[i] = fileReadHex16(file);
    switch (hextol(inputFrag[0])) {
      default:  //ERR

        println('\n');
        executing = false;
        break;

      case 0:  //NOP
        break;

      case 1:  //LDI r1 i2
        regReset(ram, hextol(inputFrag[1]));
        fileWriteHex(ram, 2);
        break;

      case 2:  //RES t1
        break;

      case 3:  //JMP_E i1 r2 r3
        inputFrag[2] = fileReadHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = fileReadHexFrom16(ram, hextol(inputFrag[3]));
        if (inputFrag[2] == inputFrag[3]) file.seek(hextol(inputFrag[1]) * (parLimit * regSize - 1)); //Se sono uguali, salta alla riga specificata d< inputFrag[1]
        break;

      case 4:  //JMP_D i1 r2 r3
        inputFrag[2] = fileReadHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = fileReadHexFrom16(ram, hextol(inputFrag[3]));

        if (inputFrag[2] != inputFrag[3]) file.seek(hextol(inputFrag[1]) * (parLimit * regSize - 1)); //Se sono diversi, salta alla riga specificata d< inputFrag[1]
        break;

      case 5:  //ADD r1 r2 r3
        regReset(ram, hextol(inputFrag[1]));
        inputFrag[2] = fileReadHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = fileReadHexFrom16(ram, hextol(inputFrag[3]));

        inputFrag[1] = ltohex(hextol(inputFrag[2]) + hextol(inputFrag[3]));
        fileWriteHex(ram, 1);
        break;

      case 6:  //SUB r1 r2 r3
        regReset(ram, hextol(inputFrag[1]));
        inputFrag[2] = fileReadHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = fileReadHexFrom16(ram, hextol(inputFrag[3]));

        inputFrag[1] = ltohex(hextol(inputFrag[2]) - hextol(inputFrag[3]));
        fileWriteHex(ram, 1);
        break;

      case 7:  //MUL r1 r2 r3
        regReset(ram, hextol(inputFrag[1]));
        inputFrag[2] = fileReadHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = fileReadHexFrom16(ram, hextol(inputFrag[3]));

        inputFrag[1] = ltohex(hextol(inputFrag[2]) * hextol(inputFrag[3]));
        fileWriteHex(ram, 1);
        break;

      case 8:  //DIV r1 r2 r3
        regReset(ram, hextol(inputFrag[1]));
        inputFrag[2] = fileReadHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = fileReadHexFrom16(ram, hextol(inputFrag[3]));

        inputFrag[1] = ltohex(hextol(inputFrag[2]) / hextol(inputFrag[3]));
        fileWriteHex(ram, 1);
        break;

      case 9:  //KEY r1
        regReset(ram, hextol(inputFrag[1]));
        if (uint8_t key = pad.getKey()) {  //Non so bene perche debba essere dichiarato come un numero, ma funziona meglio nella funzione String(key, 16)
          inputFrag[1] = ltohex(key);
          fileWriteHex(ram, 1);
        }
        break;

      case 10:  //CLS
        clearScreen();
        break;

      case 11:  //CRS_I i1 i2
        tft.setCursor(hextol(inputFrag[1]), hextol(inputFrag[2]));
        break;

      case 12:  //CRS_R r1 r2
        inputFrag[2] = fileReadHexFrom16(ram, hextol(inputFrag[1]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = fileReadHexFrom16(ram, hextol(inputFrag[2]));
        tft.setCursor(hextol(inputFrag[1]), hextol(inputFrag[2]));
        break;

      case 13:  //WRT_T i1 b2
        progPos = file.position();
        file.seek(resourceOffset);
        for (int8_t i = 0; i < hextol(inputFrag[1]) - 1; i++) {  //Se è la prima risorsa (risorsa 0) non la skippa //-1 perche la 0 è riservata //Deve avere il segno
          char1 = file.read();
          char2 = file.read();
          file.seek(file.position() + hextol(ltohex(char1) + ltohex(char2)));  //Avanza di tanto quanto la lunghezza della risorsa
        }
        char1 = file.read();
        char2 = file.read();
        for (uint16_t i = 0; i < hextol(ltohex(char1) + ltohex(char2)) && file.available(); i++) print(char(file.read()), hextol(inputFrag[2]), false);
        file.seek(progPos);
        break;

      case 14:  //WRT_I i1 b2
        print(hextol(inputFrag[1]), hextol(inputFrag[2]), false);
        break;

      case 15:  //WRT_R r1 b2
        inputFrag[1] = fileReadHexFrom16(ram, hextol(inputFrag[1]));
        print(hextol(inputFrag[1]), hextol(inputFrag[2]), false);
        break;

      case 255:  //END
        println('\n');
        executing = false;
        break;
    }

    if (Serial.available() > 0) break; //Se riceve qualcosa dalla console, chiude il programma
  }

  ram.close();
  file.close();
}

char instToChar(String inst) {     //List of AR-ASM instructions and a small description
  if (inst == "NOP") return 0;     //No operation
  if (inst == "LDI") return 1;     //Loads into [r1] the value of [i2]
  if (inst == "RES") return 2;     //USED ONLY FOR COMPILER! Saves as a new resource [t1]
  if (inst == "JMP_LE") return 3;  //Jumps to line [i1] if [r2] == [r3]
  if (inst == "JMP_OE") return 3;  //Jumps to offset [i1] if [r2] == [r3]
  if (inst == "JMP_LD") return 4;  //Jumps to line [i1] if [r2] != [r3]
  if (inst == "JMP_OD") return 4;  //Jumps to offset [i1] if [r2] != [r3]
  if (inst == "ADD") return 5;     //Saves in [r1] the value of [r2] + [r3]
  if (inst == "SUB") return 6;     //Saves in [r1] the value of [r2] - [r3]
  if (inst == "MUL") return 7;     //Saves in [r1] the value of [r2] * [r3]
  if (inst == "DIV") return 8;     //Saves in [r1] the value of [r2] / [r3]
  if (inst == "KEY") return 9;     //Saves in [r1] the value of the current pressed key at the time of call
  if (inst == "CLS") return 10;    //Clears screen
  if (inst == "CRS_I") return 11;  //Sets cursor to X = [i1] Y = [i2]
  if (inst == "CRS_R") return 12;  //Sets cursor to X = [r1] Y = [r2]
  if (inst == "WRT_T") return 13;  //Sets background as [b2] and writes to screen the resource saved to pos [i1]
  if (inst == "WRT_I") return 14;  //Sets background as [b2] and writes to screen [i1]
  if (inst == "WRT_R") return 15;  //Sets background as [b2] and writes to screen [r1]
  if (inst == "END") return 255;   //Stops execution of the program
  found = false;                   //If the function does not find an instruction, sets found to false
}
*/

void execute(File source) {
}