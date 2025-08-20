#include <SD.h>
#include <SPI.h>

void setup() {
  Serial.begin(115200);
  SD.begin(10, 11, 12, 13);

  Serial.println("\n\nStarting program!");
  uint32_t start = millis();

  //compile("/USERS/PROGRAMS/TEST.CMM", "/USERS/PROGRAMS/TEST.EXE");

  SD.remove("/USERS/PROGRAMS/TESTCOPY.TXT");
  File source = SD.open("/USERS/PROGRAMS/TEST.TXT"), destination = SD.open("/USERS/PROGRAMS/TESTCOPY.TXT", FILE_WRITE);
  fileReplace(source, destination, "test", "not-test", false, true);
  source.close();
  destination.close();

  File copy = SD.open("/USERS/PROGRAMS/TESTCOPY.TXT");
  Serial.println("Written:");
  while (copy.available()) Serial.write(copy.read());
  copy.close();

  uint32_t end = millis();
  Serial.print("\n\nCompleted in ");
  Serial.print(double(end - start) / 1000);
  Serial.print(" seconds, or ");
  Serial.print(end - start);
  Serial.print(" milliseconds.");
}

void loop() {}