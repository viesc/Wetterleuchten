#include <Adafruit_NeoPixel.h>

// LED Strip stuff
#define DATAPIN_NEOPIX1 13
#define NUMPIXELS_NEOPIX1 144 // 144
#define LEDFRINGE_NEOPIX1 0

Adafruit_NeoPixel strip1(NUMPIXELS_NEOPIX1, DATAPIN_NEOPIX1, NEO_GRBW + NEO_KHZ800);


void setup() {
  strip1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip1.show();            // Turn OFF all pixels ASAP
  strip1.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  // put your main code here, to run repeatedly:
  setWhite(1);
  delay(random(10, 100));
  turnOff(1);
  delay(random(10, 100));
}
