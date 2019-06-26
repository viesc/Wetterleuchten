void mixLedDirection() {
    Serial.println("mix led directions");
    for (int i = 0; i < NUMPIXELS_NEOPIX1; i += LEDSTEP) {
      ledDir[i] = random(2);
      Serial.print(ledDir[i]);
    }
}

void moveLedArray() {
  int step = 4;
  for (int i = step; i < NUMPIXELS_NEOPIX1-1; i += LEDSTEP) {
    if(ledMix[i] == 1) {
      if(ledDir[i] == 0) ledMix[i-step] == 1;
      else ledMix[i+step] == 1;
    }
//    Serial.print(ledMix[i]);
  }
//  Serial.println();
}

void mixLedArray() {
  // fill array with randomly placed colors based on intensity and chance
  Serial.println("mixing leds:");
  for (int i = 0; i < NUMPIXELS_NEOPIX1; i += LEDSTEP) {
    if (random(70) < intensity) ledMix[i] = 1;
    else ledMix[i] = 0;
    Serial.print(ledMix[i]);
  }
  Serial.println();
}

// fill array with randomly placed colors based on intensity
void mixLedArrayEven() {
  // setting all to white first
  for (int i = 0; i < NUMPIXELS_NEOPIX1; i += LEDSTEP) {
    ledMix[i] = 0;
  }
  // how many leds to color
  byte ledCount = map(intensity, 0, INTENSITY_MAX, 0, NUMPIXELS_NEOPIX1 / LEDSTEP);
  Serial.println(String("mixing led count:") + ledCount);
  byte j = 0;
  // randomly color until ledCount is reached
  while (j < ledCount) {
    byte ledI = random(NUMPIXELS_NEOPIX1 / LEDSTEP) * LEDSTEP;
    if (ledMix[ledI] == 0) {
      ledMix[ledI] = 1;
      j++;
    }
  }
  // output to serial
  for (int i = 0; i < NUMPIXELS_NEOPIX1; i += LEDSTEP) {
    if (ledMix[i] == 1) Serial.print(1);
    else Serial.print(0);
  }
  Serial.println();
}


void mixInHeat(byte LEDnum, byte LEDfringe, bool* ledMix) {
  for (byte i = LEDfringe; i < LEDnum - LEDfringe; i += LEDSTEP) {
    if (ledMix[i] == 1) {
      stripN1.setPixelColor(i, stripN1.Color(255, 0, 0, 0)); // mix in red pixels using simple chance
#ifdef DEBUG
      Serial.print("1");
#endif
    }
    else {
      stripN1.setPixelColor(i, stripN1.Color(0, GREEN_BRIGHT, 0, 0));
#ifdef DEBUG
      Serial.print("0");
#endif
    }
  }
#ifdef DEBUG
  Serial.println();
#endif
  stripN1.show();
}

void mixInCold(byte LEDnum, byte LEDfringe, bool* ledMix) {
  for (byte i = LEDfringe; i < LEDnum - LEDfringe; i += LEDSTEP) {
    if (ledMix[i] == 1) {
      stripN1.setPixelColor(i, stripN1.Color(0, 0, 255, 0)); // mix in blue pixels using simple chance
#ifdef DEBUG
      Serial.print("1");
#endif
    }
    else {
      stripN1.setPixelColor(i, stripN1.Color(0, GREEN_BRIGHT, 0, 0));
#ifdef DEBUG
      Serial.print("0");
#endif
    }
  }
#ifdef DEBUG
  Serial.println();
#endif
  stripN1.show();
}

void fillWhite(byte intensity) {
  stripN1.fill(stripN1.Color(0, 0, 0, stripN1.gamma8(intensity)));
  stripN1.show();
  Serial.print("*white on: ");
  Serial.println(intensity);
}

void turnOff() {
  stripN1.fill(stripN1.Color(0, 0, 0, 0));
  stripN1.show();
  //Serial.println("*lights off");
}

void testLED(byte LEDnum) {

  Serial.println("Test, test, 1, 2...");

  for (byte i = 0; i < LEDnum; i += LEDSTEP) {
    stripN1.setPixelColor(i, stripN1.Color(0, 0, 0, 255));
    stripN1.show();
#ifdef DEBUG
    Serial.print("y");
#endif
    delay(1);
  }
  delay(300);
  for (byte i = 0; i < LEDnum; i += LEDSTEP) {
    stripN1.setPixelColor(i, stripN1.Color(255, 0, 0, 0));
    stripN1.show();
#ifdef DEBUG
    Serial.print("e");
#endif
    delay(1);
  }
  delay(300);
  for (byte i = 0; i < LEDnum; i += LEDSTEP) {
    stripN1.setPixelColor(i, stripN1.Color(0, 255, 0, 0));
    stripN1.show();
#ifdef DEBUG
    Serial.print("a");
#endif
    delay(1);
  }
  delay(300);
  for (byte i = 0; i < LEDnum; i += LEDSTEP) {
    stripN1.setPixelColor(i, stripN1.Color(0, 0, 255, 0));
    stripN1.show();
#ifdef DEBUG
    Serial.print("h");
#endif
    delay(1);
  }
  delay(300);
  Serial.println("!");

  delay(200);
  turnOff();
  delay(200);
  fillWhite(255);
  delay(500);
  turnOff();
  delay(200);
  mixLedArray();
  mixInHeat(NUMPIXELS_NEOPIX1, LEDFRINGE_NEOPIX1, ledMix);
  delay(500);
  turnOff();
  delay(200);
  mixInCold(NUMPIXELS_NEOPIX1, LEDFRINGE_NEOPIX1, ledMix);
  delay(500);
  turnOff();
  delay(2000);
}

// plays LED Animation with current settings
//void UpdateLEDs (bool isDotStar, short LEDnum, short LEDfringe, short intensity, int curHue, short curSat, Adafruit_DotStar stripDS, Adafruit_NeoPixel stripNP)
void UpdateLEDs (short LEDnum, short LEDfringe, short intensity, int curHue, short curSat, Adafruit_NeoPixel stripNP)
{
  int maxValue = map(abs(intensity), 0, INTENSITY_MAX, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  unsigned short fade = map(abs(intensity), 0, INTENSITY_MAX, MIN_FADE, MAX_FADE);

  if (isOn)
  {
    value = maxValue;
    hue = curHue;
    saturation = curSat;
  }

  // fade, if glowing
  if (!isOn && value > 0)
  {
    value = max(value - fade, 0);
    hue = (hue + 400) % 65535;
    saturation = max((int)(saturation - fade / 10.0), 0);
  }

  // turn on with random chance
  if (!isOn && random(0, map(abs(intensity), 0, INTENSITY_MAX, MIN_FLICKER_ON, MAX_FLICKER_ON)) == 0)
  {
    isOn = true;
    value = maxValue;
    hue = curHue;
    saturation = curSat;
  }

  // turn off with random chance
  if (isOn && random(0, map(abs(intensity), 0, INTENSITY_MAX, MIN_FLICKER_OFF, MAX_FLICKER_OFF)) == 0)
  {
    isOn = false;
  }

  for (byte i = LEDfringe; i < LEDnum - LEDfringe; i += LEDSTEP)
  {
    float weirdShit1 = sin((float)i / 40.0 * (float)tDay / 1000.0);
    float weirdShit2 = sin((float)i / 10.0 * (float)tDay / 500.0);

    int finValue = max(0, (int)(value - weirdShit1 * 10 - weirdShit2 * maxValue / 20.0));
    stripNP.setPixelColor(i, stripNP.ColorHSV(255, saturation, finValue));
    Serial.println(i);
  }

  stripNP.show();

  Serial.println(String("value: ") + value);
}
