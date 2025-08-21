#include <SD.h>
#include <SPI.h>

#define DEBUG

String tempDir = "/";
uint32_t tempCount = 0;

void setup() {
  Serial.begin(115200);
  SD.begin(10, 11, 12, 13);

  Serial.println("\n\nStarting...");
  uint32_t start = millis();

  //compile("/USERS/PROGRAMS/TEST.CMM", "/USERS/PROGRAMS/TEST.EXE");

  /*
  SD.remove("/USERS/PROGRAMS/TESTCOPY.TXT");
  File source = SD.open("/USERS/PROGRAMS/TEST.TXT"), destination = SD.open("/USERS/PROGRAMS/TESTCOPY.TXT", FILE_WRITE);
  fileReplace(source, destination, "test", "not-test", false, true);
  source.close();
  destination.close();
  */

  String testFilePath = "/USERS/PROGRAMS/TESTRPLC.TXT";                                                                                  //Saves the test file path
  SD.remove(testFilePath.c_str());                                                                                                       //Deletes the test file, if existant
  File file = SD.open(testFilePath.c_str(), FILE_WRITE);                                                                                 //Creates it
  file.print("test\nThis is a new test for testing the replace function\nThis is a default text file\nTest TEST test teSt test\ntest");  //Writes default text
  file.close();                                                                                                                          //Saves the text
  fileReplace(testFilePath, testFilePath, "test", "not-test", true, true);                                                               //Starts replacing process
  //Replaces all the instances of "test" with "not-test"
  //It makes sure that "test" is an whole word and not inside of another word
  //And it's case sensitive

  uint32_t end = millis();
  Serial.print("\n\nCompleted in ");
  Serial.print(double(end - start) / 1000);
  Serial.print(" seconds, or ");
  Serial.print(end - start);
  Serial.print(" milliseconds!");
}

void loop() {}