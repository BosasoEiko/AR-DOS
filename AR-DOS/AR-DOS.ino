//SCREEN----------------------------------------------------------------------

#include <Elegoo_GFX.h>
#include <Elegoo_TFTLCD.h>
Elegoo_TFTLCD tft(A3, A2, A1, A0, A4);

#define BLACK 0x0000
#define WHITE 0xFFFF

int colorText = WHITE;
int colorBack = BLACK;

const uint8_t charX = 52;                 //contando lo 0
const uint8_t charY = 29;                 //contando lo 0
char screenBuffer[charX + 1][charY + 1];  //perche la definizione non conta lo 0
bool bg = false;
bool invert = false;

//SD----------------------------------------------------------------------

#include <SPI.h>
#include <SD.h>

uint8_t diskPin[4] = { 10, 11, 12, 13 };  //SS, DI, DO, CLK
char diskSel = 'A';

File root = SD.open("/");
File currentDir = root;
String currentPath = String(diskSel) + String(':') + String(currentDir.name());

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

char keyNormal[10][4] = { { ' ', ',', '.', '0' },
                          { 'a', 'b', 'c', '1' },
                          { 'd', 'e', 'f', '2' },
                          { 'g', 'h', 'i', '3' },
                          { 'j', 'k', 'l', '4' },
                          { 'm', 'n', 'o', '5' },
                          { 'p', 'q', 'r', '6' },
                          { 's', 't', 'u', '7' },
                          { 'v', 'w', 'x', '8' },
                          { 'y', 'z', '\\', '9' } };

char keyShift[10][4] = { { '\\', ';', ':', '=' },
                         { 'A', 'B', 'C', '!' },
                         { 'D', 'E', 'F', '"' },
                         { 'G', 'H', 'I', '\'' },
                         { 'J', 'K', 'L', '$' },
                         { 'M', 'N', 'O', '%' },
                         { 'P', 'Q', 'R', '&' },
                         { 'S', 'T', 'U', '/' },
                         { 'V', 'W', 'X', '(' },
                         { 'Y', 'Z', '?', ')' } };

char keyPressed;  //Pubblica a tutte le funzioni

const uint8_t parameters = 4;
String inputSaved, inputFrag[parameters];  //Pubblica per un casino di funzioni che usano frag() e input()

//AUDIO----------------------------------------------------------------------

uint8_t audioPin = 48;
uint16_t audioHz = 400;

//OTHER---------------------------------------------------------------------

bool on = false;
bool loadedGui = false;  //per vedere se è stata stampata la GUI
bool printed = false;    //la utilizzano cmd() e printChar()
bool found;              //Usato da cmp() e instToChar()

//START---------------------------------------------------------------------

void setup() {
  on = true;
  set("SET.SYS", "");  //Carica le impostazioni di sistema all'avvio
  Serial.begin(9600);

  uint16_t ID = tft.readID();
  tft.begin(ID);
  cls();
  tft.setRotation(3);

  print(F("Loading screen driver..."), true);
  print(F("Loaded screen driver with ID: 0x"), false);
  print(ID, HEX, true);
  print(true);

  print(F("Loading audio driver..."), true);
  pinMode(audioPin, OUTPUT);
  tone(audioPin, audioHz, 1000);
  print(F("Loaded audio driver connected to port: 48\n"), true);

  print(F("Loading keypad driver..."), true);
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

  print(F("Loading disk driver..."), true);
  if (SD.begin(diskPin[0], diskPin[1], diskPin[2], diskPin[3])) {
    print(F("Loaded disk driver connected to ports:\nSS: "), false);
    print(diskPin[0], false);
    print(F(", DI: "), false);
    print(diskPin[1], false);
    print(F(", DO: "), false);
    print(diskPin[2], false);
    print(F(", CLK: "), false);
    print(diskPin[3], true);
  } else {
    print(F("Cannot load disk driver connected to ports:\nSS: "), false);
    print(diskPin[0], false);
    print(F(", DI: "), false);
    print(diskPin[1], false);
    print(F(", DO: "), false);
    print(diskPin[2], false);
    print(F(", CLK: "), false);
    print(diskPin[3], true);
  }
  print(true);

  print(F("Loading extended memory driver..."), true);
  int start = millis();
  SD.remove("RAM.SYS");
  File ram = SD.open("RAM.SYS", FILE_WRITE);
  for (uint16_t i = 0; i < 16384; i++) ram.print(char(0));  //16kb ram
  int end = millis();
  if (ram) {
    print(F("Loaded 16KB of extended memory in "), false);
    print(end - start, false);
    print(F(" millis"), true);
  }
  else print(F("Cannot load extended memory driver"), true);
  ram.close();
  set("SET.SYS", "");  //Carica le impostazioni di sistema all'avvio
  while (on) cmd();
}

void loop() {}