bool isChar(int16_t c) {
  switch (c) {
    default:
#ifdef DEBUG
      Serial.print("[CHAR: ");
      Serial.print(c);
      Serial.print("]");
#endif
      return true;
    case -1:
    case 0:
    case 13:  //CR: Carriage Return
    case ' ':
    case '\t':
    case '\n':
#ifdef DEBUG
      Serial.print("[NOT CHAR]");
#endif
      return false;
  }
}