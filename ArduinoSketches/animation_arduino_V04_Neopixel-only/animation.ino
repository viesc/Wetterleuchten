void mixInHeat(byte intensity, byte LEDnum, byte LEDfringe) {
  Serial.println("*mixing in hot pixels");
  for (byte i = LEDfringe; i < LEDnum - LEDfringe; i += LEDSTEP) {
    if (random(100) < intensity) {
      stripN1.setPixelColor(i, stripN1.Color(255, 0, 0, 0)); // mix in red pixels using simple chance
#ifdef DEBUG
      Serial.print("1");
#endif
    }
    else {
      stripN1.setPixelColor(i, stripN1.Color(0, 0, 0, 255));
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

void mixInCold(byte intensity, byte LEDnum, byte LEDfringe) {
  Serial.println("*mixing in cold pixels");
  for (byte i = LEDfringe; i < LEDnum - LEDfringe; i += LEDSTEP) {
    if (random(100) < intensity) {
      stripN1.setPixelColor(i, stripN1.Color(0, 0, 255, 0)); // mix in blue pixels using simple chance
#ifdef DEBUG
      Serial.print("1");
#endif
    }
    else {
      stripN1.setPixelColor(i, stripN1.Color(0, 0, 0, 255));
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
