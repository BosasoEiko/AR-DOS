/*
struct Program {
  File exe, mem;
  uint32_t PC;         //Program counter
  uint8_t size, mode;  //Size of the parameters in bytes //Mode of execution of the progam (EXPERIMENTAL)
  uint32_t data;       //Data offset
  uint8_t prog;        //Program offset
  uint8_t PID;         //Program ID

  void init(String path) {  //Initializes the pogram
    if (exe = fileOpen(path)) PID = programCounter += 1;
    else return;
    mem = fileOpen(pathSum(memDir, ));
    size = hexRead(exe, 1);
    mode = hexRead(exe, 1);
    data = hexRead(exe, 4);
    prog = filePos(exe) + (size - 1) * 4;
    PC = 0;
    fileSeek(exe, prog);  //Jumps to the start of the program
  }

  void inst() {                                                //Executes an instruction
    uint32_t par[4];                                           //4 parameters
    uint8_t inst = hexRead(exe, 1), type = hexRead(exe, 1);    //Read instruction and parameter's type
    for (uint8_t i = 0; i < 4; i++) par[i] = hexRead(exe, 4);  //Reads each parameter

    switch (inst) {
      default:
        break;
      case 0x00:  //No operation
        break;
    }
  }

  uint32_t hexRead(File &file, uint8_t bytes) {  //Reads a number of bytes in a file, and returns the integer value of the hex string read
    String hex = "";
    for (bytes; bytes != 0; i++) hex += String(file.read(), 16);
    return strtol(hex.c_str(), NULL, 16);
  }

  void hexWrite(File &file, uint32_t value, uint8_t bytes) {}  //TODO

  void fileWriteHex(File &dir, uint8_t i) {  //It's here because i have no idea how it works
    if (inputFrag[i].length() < 4) {         //Standardizza la stringa a 4 caratteri (16bit)
      String temp = inputFrag[i];
      inputFrag[i] = "";
      while (inputFrag[i].length() + temp.length() <= 4) inputFrag[i] += "0";
      inputFrag[i] += temp;
    }
    if (inputFrag[i].length() > 4) inputFrag[i] = inputFrag[i].substring(inputFrag[i].length() - 4, inputFrag[i].length());

    dir.print(char(hextol(inputFrag[i].substring(0, 2))));  //Converte la prima sezione in una stringa C, converte da esadecimale a decimale, converte in un carattere e lo stampa
    dir.print(char(hextol(inputFrag[i].substring(2, 4))));  //Converte la seconda sezione in una stringa C, converte da esadecimale a decimale, converte in un carattere e lo stampa
  }
};
*/