void exit() {
  cls();
  //ram.close();
  tft.setTextSize(2);
  tft.print(F("It's now safe to turn off\nyour computer."));
  on = false;
}