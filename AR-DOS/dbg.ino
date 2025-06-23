void dbg() {
  Serial.println("DEBUG\n\n--------------");
  File file = fileOpen("//TEST/DOUBLE/TESTING.TXT", FILE_WRITE);
  file.close();
  SD.mkdir("C:");
}