void testConvs() {
  Serial.println(i2hex(69, 4));
  Serial.println(hex2ui(i2hex(69, 4)));
  Serial.println(i2hex(-69, 4));
  Serial.println(hex2ui(i2hex(-69, 4)));
  Serial.println(i2hex(4294967295, 4));
  Serial.println(i2hex(2147483647, 4));
  Serial.println(i2hex(-2147483648, 4));
  Serial.println(hex2i("80000000"));
  Serial.println(hex2ui("80000000"));
  Serial.println(hex2i("ffffffff"));
  Serial.println(hex2ui("ffffffff"));
}

void test(File& testFile) {
  Serial.println("-- IN FUNCTION --");
  Serial.println(testFile.position());
  testFile.print("Test");
  Serial.println(testFile.position());
  Serial.println("-- OUT FUNCTION --");
}

void test2(File testFile) {
  Serial.println("-- IN FUNCTION 2 --");
  Serial.println(testFile.position());
  testFile.print("Test");
  Serial.println(testFile.position());
  Serial.println("-- OUT FUNCTION 2 --");
}

void fileTests() {
  SD.remove("TEST.TXT");
  File outsideFile = SD.open("TEST.TXT", FILE_WRITE);
  outsideFile.print("Outside Test\n");
  Serial.println(outsideFile.position());
  test(outsideFile);
  Serial.println(outsideFile.position());
  Serial.println("READING");
  outsideFile.seek(0);
  while (outsideFile.available()) Serial.write(outsideFile.read());
  Serial.println("\nREADING FINISHED");

  if (1) {
    File file1 = SD.open("TEST.TXT"), file2 = SD.open("TEST.TXT");
    if (file1 == file2) Serial.println("SAME");
    else Serial.println("DIFFERENT");
    file1.close();
    file2.close();
  }

  if (1) {
    File file1 = SD.open("TEST.TXT"), file2 = file1;
    if (file1 == file2) Serial.println("SAME");
    else Serial.println("DIFFERENT");
    file1.close();
    file2.close();
  }

  if (1) {
    File sameFile = SD.open("TEST.TXT");
    File file1 = sameFile, file2 = sameFile;
    if (file1 == file2) Serial.println("SAME");
    else Serial.println("DIFFERENT");
    file1.close();
    file2.close();
  }
}