void setWhite(int step) {
  for (int i = 0; i < NUMPIXELS_NEOPIX1; i += step) {
    strip1.setPixelColor(i, strip1.Color(0, 0, 0, 255));
  }
  strip1.show();
}

void turnOff(int step) {
  for (int i = 0; i < NUMPIXELS_NEOPIX1; i += step) {
    strip1.setPixelColor(i, strip1.Color(0, 0, 0, 0));
  }
  strip1.show();
}
