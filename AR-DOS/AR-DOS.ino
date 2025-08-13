//SCREEN----------------------------------------------------------------------

#include <Elegoo_TFTLCD.h>  //Comunication with TFT screen
#include <Elegoo_GFX.h>     //Graphics tools
Elegoo_TFTLCD tft(A3, A2, A1, A0, A4);

uint16_t colorText = 0xFFFF, colorBack = 0x0000;                      //White //Black
const uint8_t charX = 52, charY = 29, charWidth = 6, charHeight = 8;  //Amount of chars that the screen can display on the X (counting the 0) //Same for Y
char textBuffer[charX + 1][charY + 1];                                //Screen buffer saves every text on the screen (declaring doesn't count the 0, so add 1 to x and y)

//SD---------------------------------------------------------------------

#include <SPI.h>
#include <SD.h>

const String sysDisk = "C:";                       //System disk
String diskSel = sysDisk;                          //Current disk
String pathSum(String, String);                    //Declaration of used function before it being loaded
const String rootDir = sysDisk;                    //Root directory
const String sysDir = pathSum(rootDir, "SYSTEM");  //System directory
const String memDir = pathSum(sysDir, "MEMORY");   //Virtual program memory directory

String currentPath = rootDir;  //Default starting dir

//KEYBOARD----------------------------------------------------------------------

#include <PS2KeyAdvanced.h>  //Comunication with PS/2 keyboard
#include <PS2KeyMap.h>       //International key maps

PS2KeyAdvanced kb;
PS2KeyMap km;

const uint8_t parLimit = 255;            //Available commands parameters
String inputSaved, inputFrag[parLimit];  //Raw input from command line //Fragmented raw input in parameters

//AUDIO----------------------------------------------------------------------

uint8_t speakerPin = 50;
uint16_t speakerHz = 400;

//OS------------------------------------------------------------------------

bool printed = false, found = false;
uint8_t progCounter = 0;

//START---------------------------------------------------------------------

void setup() {  //Basically the BIOS
  Serial.begin(115200);

  uint16_t ID = tft.readID();
  if (ID) {
    tft.begin(ID);
    tft.setRotation(3);  //Landscape mode
    clearScreen();
    print(F("Found display with ID: 0x"));
    println(ID, 16);
  } else Serial.println(F("Cannot find display"));

  pinMode(speakerPin, INPUT_PULLUP);    //To check if the pin is connected to ground, aka there's a device
  if (!digitalRead(speakerPin)) {       //1 if it's not connected, 0 if it is connected, so it needs to be inverted
    tone(speakerPin, speakerHz, 1000);  //BIIIP
    print(F("Found speaker on port: "));
  } else print(F("Cannot find speaker on port: "));
  println(speakerPin);

  if (SD.begin(10, 11, 12, 13)) print(F("Found disk on port: "));  //SS, DI, DO, CLK
  else print(F("Cannot find disk on port: "));
  println(10);  //SS

  kb.begin(21, 20);
  kb.setNoBreak(1);
  kb.setNoRepeat(1);
  km.selectMap("IT");
  print(F("Press any key to continue..."));
  while(!kb.available()) {};

  clearScreen();
  tone(speakerPin, speakerHz, 250);
  println(F("\nAR-DOS 1.1\n"));  //Name and version
  cmd();                         //Loads the command-line GUI
}

void loop() {}  //SULAS