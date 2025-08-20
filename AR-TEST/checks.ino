bool isChar(int16_t c) {
  switch (c) {
    default:
      Serial.print("[CHAR: ");
      Serial.print(c);
      Serial.print("]");
      return true;
    case -1:
    case 0:
    case 13:  //CR: Carriage Return
    case ' ':
    case '\t':
    case '\n':
      Serial.print("[NOT CHAR]");
      return false;
  }
}