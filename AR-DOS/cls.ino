void cls() {
  tft.fillScreen(colorBack);
  tft.setCursor(0, 0);
  for (uint8_t y = 0; y < charY; y++) {
    for (uint8_t x = 0; x < charX; x++) {
      screenBuffer[x][y] = ' ';
    }
  }
}