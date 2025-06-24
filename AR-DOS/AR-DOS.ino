//SCREEN----------------------------------------------------------------------

#include <Elegoo_GFX.h>
#include <Elegoo_TFTLCD.h>
Elegoo_TFTLCD tft(A3, A2, A1, A0, A4);

uint16_t colorText = 0xFFFF, colorBack = 0x0000;  //White //Black

const uint8_t charX = 52, charY = 29;   //Amount of chars that the screen can display on the X (counting the 0) //Same for Y
char textBuffer[charX + 1][charY + 1];  //Screen buffer saves every text on the screen (declaring doesn't count the 0, so add 1 to x and y)

//SD----------------------------------------------------------------------

#include <SPI.h>
#include <SD.h>

const String sysDisk = "C:";  //System disk
String diskSel = sysDisk;     //Current disk

const String rootDir = sysDisk;             //Root directory (EXPERIMENTAL)
const String sysDir = rootDir + "/SYSTEM";  //System directory (EXPERIMENTAL)

const String ramFile = sysDir + "/RAM.SYS";  //Extended memory file (EXPERIMENTAL)
uint8_t ramSize = 16;                        //Extended memory file size in KB (EXPERIMENTAL)

const String setFile = sysDir + "/SET.SYS";  //Settings file (EXPERIMENTAL)

String currentPath = rootDir;  //Default starting dir

const uint8_t regSize = 2;  //In bytes

//KEYPAD----------------------------------------------------------------------

#include <Keypad.h>

const uint8_t ROWS = 4, COLS = 4;

char keys[ROWS][COLS] = { { '1', '2', '3', 'A' },
                          { '4', '5', '6', 'B' },
                          { '7', '8', '9', 'C' },
                          { '*', '0', '#', 'D' } };

const uint8_t rowPins[ROWS] = { 22, 24, 26, 28 }, colPins[COLS] = { 23, 25, 27, 29 };

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

const uint8_t parameters = 4;              //Available commands parameters (Variable in future release (maybe))
String inputSaved, inputLast, inputFrag[parameters];  //Raw input from command line //Fragmented raw input in parameters

//AUDIO----------------------------------------------------------------------

uint8_t speakerPin = 50;
uint16_t speakerHz = 400;

//OS------------------------------------------------------------------------

bool on, printed = false, found = false;

//START---------------------------------------------------------------------

void setup() {
  on = true;  //Booted OS
  Serial.begin(9600);

  uint16_t ID = tft.readID();
  if (ID) {
    tft.begin(ID);
    tft.setRotation(3);      //Landscape mode
    tft.setTextWrap(false);  //Text overflows (I will add warp to my print API)
    cls();
    print(F("Found display with ID: 0x"));
    println(ID, 16);
  } else Serial.println(F("Cannot find display"));

  /* Not needed, speaker pin is constant (maybe will change in the future)
  for (speakerPin = 48; speakerPin <= 53; speakerPin++) {  //Automatic audio port detection
    pinMode(speakerPin, INPUT_PULLUP);
    if (!digitalRead(speakerPin)) {
      tone(speakerPin, speakerHz, 1000);
      print(F("Found speaker on port: "), false);
      print(speakerPin, true);
      break;
    }
    if (speakerPin == 53) {
      speakerPin = -1;
      print(F("Cannot find speaker port"), true);
      break;
    }
  }
  */

  pinMode(speakerPin, INPUT_PULLUP);  //To check if the pin is connected to ground, aka there's a device
  if (!digitalRead(speakerPin)) {     //1 if it's not connected, 0 if it is connected, so it needs to be inverted
    tone(speakerPin, speakerHz, 1000);
    print(F("Found speaker on port: "));
  } else print(F("Cannot find speaker on port: "));
  println(speakerPin);

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

  if (SD.begin(10, 11, 12, 13)) {  //SS, DI, DO, CLK
    dirMake(sysDir);               //Creates system folder
    print(F("Found disk on port: "));
  } else print(F("Cannot find disk on port: "));
  println(10);  //SS

  int start = millis();  //Extended memory reset
  fileRemove(ramFile);
  File ram = fileOpen(ramFile, FILE_WRITE);
  for (uint32_t i = 0; i < ramSize * 1024; i++) ram.print(char(0));  //Ram file reset (fill with 0)
  int end = millis();
  if (ram) {
    print(F("Loaded "));
    print(ramSize);
    print(F("KB of extended memory in "));
    print(end - start);
    println(F(" ms"));
  } else println(F("Cannot load extended memory driver"));
  ram.close();

  set("load", "");  //Loads settings with screen refresh

  tone(speakerPin, speakerHz, 250);
  println(F("\nAR-DOS 1.1\n"));  //Name and version

  while (on) cmd();  //Loads the command-line GUI
}

void loop() {}  //SULAS