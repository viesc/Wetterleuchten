// plays LED Animation with current settings
//void UpdateLEDs (bool isDotStar, short LEDnum, short LEDfringe, short intensity, int curHue, short curSat, Adafruit_DotStar stripDS, Adafruit_NeoPixel stripNP)
void UpdateLEDs (bool isDotStar, short LEDnum, short LEDfringe, short intensity, int curHue, short curSat, Adafruit_NeoPixel stripNP)
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
    hue = (hue + 400)%65535;
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

  for (byte i=LEDfringe; i<LEDnum-LEDfringe; i+=LEDSTEP)
  {
    float weirdShit1 = sin((float)i / 40.0 * (float)tDay/1000.0);
    float weirdShit2 = sin((float)i / 10.0 * (float)tDay/500.0);

    int finValue = max(0, (int)(value - weirdShit1 * 10 - weirdShit2 * maxValue / 20.0));

    if (isDotStar)
    {
      //stripDS.setPixelColor(i, stripDS.ColorHSV(hue, saturation, finValue));
    }
    else
    {
      stripNP.setPixelColor(i, stripNP.ColorHSV(255, saturation, finValue));
      //stripNP.setPixelColor(i, 0, 255, 0);
    }
  }

  if (isDotStar)
  {
    //stripDS.show();
  }
  else
  {
    stripNP.show();
  }

  //Serial.println(String("value: ") + value);
}



void testLED(byte LEDnum, byte LEDpin, Adafruit_NeoPixel stripNP) {

  for (byte i = 0; i < LEDnum; i += LEDSTEP) {
    stripNP.setPixelColor(i, stripNP.Color(0, 0, 0, 255));
    stripNP.show();
    delay(1);
  }
  delay(300);
  for (byte i = 0; i < LEDnum; i += LEDSTEP) {
    stripNP.setPixelColor(i, stripNP.Color(255, 0, 0, 0));
    stripNP.show();
    delay(1);
  }
  delay(300);
  for (byte i = 0; i < LEDnum; i += LEDSTEP) {
    stripNP.setPixelColor(i, stripNP.Color(0, 255, 0, 0));
    stripNP.show();
    delay(1);
  }
  delay(300);
  for (byte i = 0; i < LEDnum; i += LEDSTEP) {
    stripNP.setPixelColor(i, stripNP.Color(0, 0, 255, 0));
    stripNP.show();
    delay(1);
  }
  delay(300);
}
