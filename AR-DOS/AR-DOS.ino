//SCREEN----------------------------------------------------------------------

#include <Elegoo_GFX.h>
#include <Elegoo_TFTLCD.h>
Elegoo_TFTLCD tft(A3, A2, A1, A0, A4);

#define BLACK 0x0000
#define WHITE 0xFFFF

int colorText = WHITE;
int colorBack = BLACK;

const uint8_t charX = 52;                 //Counting the 0
const uint8_t charY = 29;                 //Counting the 0
char screenBuffer[charX + 1][charY + 1];  //Definition doesn't count the 0, so add 1
bool bg = false;
bool invert = false;

//SD----------------------------------------------------------------------

#include <SPI.h>
#include <SD.h>

String diskSel = "C";

const String rootDir = "/";                 //Root directory (EXPERIMENTAL)
const String sysDir = rootDir + "/SYSTEM";  //System directory (EXPERIMENTAL)

const String ramFile = sysDir + "/RAM.SYS";  //Extended memory file (EXPERIMENTAL)
uint8_t ramSize = 16;                       //Extended memory file size in KB (EXPERIMENTAL)

const String setFile = sysDir + "/SET.SYS";  //Settings file (EXPERIMENTAL)

String currentPath = rootDir; //Default starting dir

const uint8_t regSize = 2;  //In bytes

//KEYPAD----------------------------------------------------------------------

#include <Keypad.h>

const uint8_t ROWS = 4;
const uint8_t COLS = 4;

char keys[ROWS][COLS] = { { '1', '2', '3', 'A' },
                          { '4', '5', '6', 'B' },
                          { '7', '8', '9', 'C' },
                          { '*', '0', '#', 'D' } };

const uint8_t rowPins[ROWS] = { 22, 24, 26, 28 };
const uint8_t colPins[COLS] = { 23, 25, 27, 29 };

Keypad pad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char keyNormal[10][4] = { { ' ', ',', '.', '0' },  //Normal mode
                          { 'a', 'b', 'c', '1' },
                          { 'd', 'e', 'f', '2' },
                          { 'g', 'h', 'i', '3' },
                          { 'j', 'k', 'l', '4' },
                          { 'm', 'n', 'o', '5' },
                          { 'p', 'q', 'r', '6' },
                          { 's', 't', 'u', '7' },
                          { 'v', 'w', 'x', '8' },
                          { 'y', 'z', '\\', '9' } };

char keyShift[10][4] = { { '\\', ';', ':', '=' },  //Shift mode
                         { 'A', 'B', 'C', '!' },
                         { 'D', 'E', 'F', '"' },
                         { 'G', 'H', 'I', '\'' },
                         { 'J', 'K', 'L', '$' },
                         { 'M', 'N', 'O', '%' },
                         { 'P', 'Q', 'R', '&' },
                         { 'S', 'T', 'U', '/' },
                         { 'V', 'W', 'X', '(' },
                         { 'Y', 'Z', '?', ')' } };

const uint8_t parameters = 4;              //Available commands parameters (Variable in future release)
String inputSaved, inputFrag[parameters];  //Raw input from command line //Fragmented raw input in parameters

//AUDIO----------------------------------------------------------------------

uint8_t audioPin = 50;
uint16_t audioHz = 400;

//OS------------------------------------------------------------------------

bool on = false;
bool printed = false;  //Public for cmd() and printChar()
bool found;            //Compiler checks if the instruction exists //Public for cmp() and instToChar()

//START---------------------------------------------------------------------

void setup() {
  on = true;  //Booted OS
  Serial.begin(9600);

  uint16_t ID = tft.readID();
  if (ID) {
    tft.begin(ID);
    tft.setRotation(3);  //Landscape mode
    cls();
    print(F("Found display with ID: 0x"), false);
    print(ID, 16, true);
  } else Serial.println(F("Cannot find display"));

  /* Not needed, speaker pin is constant (maybe will change in the future)
  for (audioPin = 48; audioPin <= 53; audioPin++) {  //Automatic audio port detection
    pinMode(audioPin, INPUT_PULLUP);
    if (!digitalRead(audioPin)) {
      tone(audioPin, audioHz, 1000);
      print(F("Found speaker on port: "), false);
      print(audioPin, true);
      break;
    }
    if (audioPin == 53) {
      audioPin = -1;
      print(F("Cannot find speaker port"), true);
      break;
    }
  }
  */

  pinMode(audioPin, INPUT_PULLUP);  //To check if the pin is connected to ground, aka there's a device
  if (!digitalRead(audioPin)) {     //1 if it's not connected, 0 if it is connected, so it needs to be inverted
    tone(audioPin, audioHz, 1000);
    print(F("Found speaker on port: "), false);
  } else print(F("Cannot find speaker on port: "), false);
  print(audioPin, true);

  /* Not needed, only for GUI
  print(F("Loaded keypad driver connected to ports: "), true);
  print(F("ROWS:"), false);
  for (uint8_t i = 0; i < ROWS; i++) {
    print(F(" "), false);
    print(rowPins[i], false);
  }
  print(F(", COLS:"), false);
  for (uint8_t i = 0; i < COLS; i++) {
    print(F(" "), false);
    print(colPins[i], false);
  }
  print(F("\n"), true);
  */

  if (SD.begin(10, 11, 12, 13)) print(F("Found disk on port: "), false);  //SS, DI, DO, CLK
  else print(F("Cannot find disk on port: "), false);
  print(10, true);  //SS

  int start = millis();  //Extended memory reset
  removeFile(ramFile);
  File ram = openFile(ramFile, FILE_WRITE);
  for (uint32_t i = 0; i < ramSize * 1024; i++) ram.print(char(0));  //Ram file reset (fill with 0)
  int end = millis();
  if (ram) {
    print(F("Loaded "), false);
    print(ramSize, false);
    print(F("KB of extended memory in "), false);
    print(end - start, false);
    print(F(" ms"), true);
  } else print(F("Cannot load extended memory driver"), true);
  ram.close();

  set(setFile, "");  //Carica le impostazioni di sistema all'avvio
  while (on) cmd();  //Loads the command-line GUI
}

void loop() {}