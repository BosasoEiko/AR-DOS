void run(String filePar) {
  File file = fileOpen(filePar, O_READ);
  File ram = fileOpen(ramFile, FILE_WRITE);

  for (uint16_t i = 0; i < 16384; i++) ram.print(char(0)); //Resetta la ram
  ram.seek(0);
  file.seek(1);

  uint8_t char1, char2;
  uint16_t progPos;

  inputFrag[1] = readHex16(file);
  uint16_t resourceOffset = hextol(inputFrag[1]);

  file.seek(7);  //Vai alla linea dove inizia il programma

  bool executing = true;

  while (file.available() && executing) {
    inputFrag[0] = readHex8(file);
    for (uint8_t i = 1; i < parameters && file.available(); i++) inputFrag[i] = readHex16(file);
    switch (hextol(inputFrag[0])) {
      default:  //ERR

        println('\n');
        executing = false;
        break;

      case 0:  //NOP
        break;

      case 1:  //LDI r1 i2
        regReset(ram, hextol(inputFrag[1]));
        printHex(ram, 2);
        break;

      case 2:  //RES t1
        break;

      case 3:  //JMP_E i1 r2 r3
        inputFrag[2] = readHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = readHexFrom16(ram, hextol(inputFrag[3]));
        if (inputFrag[2] == inputFrag[3]) file.seek(hextol(inputFrag[1]) * (parameters * regSize - 1)); //Se sono uguali, salta alla riga specificata d< inputFrag[1]
        break;

      case 4:  //JMP_D i1 r2 r3
        inputFrag[2] = readHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = readHexFrom16(ram, hextol(inputFrag[3]));

        if (inputFrag[2] != inputFrag[3]) file.seek(hextol(inputFrag[1]) * (parameters * regSize - 1)); //Se sono diversi, salta alla riga specificata d< inputFrag[1]
        break;

      case 5:  //ADD r1 r2 r3
        regReset(ram, hextol(inputFrag[1]));
        inputFrag[2] = readHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = readHexFrom16(ram, hextol(inputFrag[3]));

        inputFrag[1] = ltohex(hextol(inputFrag[2]) + hextol(inputFrag[3]));
        printHex(ram, 1);
        break;

      case 6:  //SUB r1 r2 r3
        regReset(ram, hextol(inputFrag[1]));
        inputFrag[2] = readHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = readHexFrom16(ram, hextol(inputFrag[3]));

        inputFrag[1] = ltohex(hextol(inputFrag[2]) - hextol(inputFrag[3]));
        printHex(ram, 1);
        break;

      case 7:  //MUL r1 r2 r3
        regReset(ram, hextol(inputFrag[1]));
        inputFrag[2] = readHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = readHexFrom16(ram, hextol(inputFrag[3]));

        inputFrag[1] = ltohex(hextol(inputFrag[2]) * hextol(inputFrag[3]));
        printHex(ram, 1);
        break;

      case 8:  //DIV r1 r2 r3
        regReset(ram, hextol(inputFrag[1]));
        inputFrag[2] = readHexFrom16(ram, hextol(inputFrag[2]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = readHexFrom16(ram, hextol(inputFrag[3]));

        inputFrag[1] = ltohex(hextol(inputFrag[2]) / hextol(inputFrag[3]));
        printHex(ram, 1);
        break;

      case 9:  //KEY r1
        regReset(ram, hextol(inputFrag[1]));
        if (uint8_t key = pad.getKey()) {  //Non so bene perche debba essere dichiarato come un numero, ma funziona meglio nella funzione String(key, 16)
          inputFrag[1] = ltohex(key);
          printHex(ram, 1);
        }
        break;

      case 10:  //CLS
        cls();
        break;

      case 11:  //CRS_I i1 i2
        tft.setCursor(hextol(inputFrag[1]), hextol(inputFrag[2]));
        break;

      case 12:  //CRS_R r1 r2
        inputFrag[2] = readHexFrom16(ram, hextol(inputFrag[1]));  //Legge 2 byte dalla RAM alla posizione definita da inputFrag, convertito da Hex a Long, e lo salva come stringa nello stesso inputFrag
        inputFrag[3] = readHexFrom16(ram, hextol(inputFrag[2]));
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
        inputFrag[1] = readHexFrom16(ram, hextol(inputFrag[1]));
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