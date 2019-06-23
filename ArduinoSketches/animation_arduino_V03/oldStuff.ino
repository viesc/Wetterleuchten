short lastIntensity = 0;
short GetIntensityFromPoti()
{
  // TEMP get intensity from potentiometer
  short intensity = constrain(map(analogRead(A0), 0, 650, -INTENSITY_MAX, INTENSITY_MAX), -INTENSITY_MAX, INTENSITY_MAX);

  if (intensity != lastIntensity)
  {
    Serial.println(intensity);
    lastIntensity = intensity;
  }

  return intensity;
}



/* OLD STUFF

  void HSVtoRGB(int hue, int sat, int val, int colors[3])
  {
  // hue: 0-359, sat: 0-255, val (lightness): 0-255
  int r, g, b, base;
  if (sat == 0)
    {                     // Achromatic color (gray).
    colors[0] = val;
    colors[1] = val;
    colors[2] = val;
    }
  else
    {
    base = ((255 - sat) * val) >> 8;
    switch(hue / 60)
      {
      case 0:
        r = val;
        g = (((val - base) * hue) / 60) + base;
        b = base;
        break;
      case 1:
        r = (((val - base) * (60 - (hue % 60))) / 60) + base;
        g = val;
        b = base;
        break;
      case 2:
        r = base;
        g = val;
        b = (((val - base) * (hue % 60)) / 60) + base;
        break;
      case 3:
        r = base;
        g = (((val - base) * (60 - (hue % 60))) / 60) + base;
        b = val;
        break;
      case 4:
        r = (((val - base) * (hue % 60)) / 60) + base;
        g = base;
        b = val;
        break;
      case 5:
        r = val;
        g = base;
        b = (((val - base) * (60 - (hue % 60))) / 60) + base;
        break;
      }
      colors[0] = r;
      colors[1] = g;
      colors[2] = b;
    }
  }

  void PlayAnimation (short intensity)
  {
  short value = 0;

  // turn on
  for (byte i=0; i<NUMPIXELS; i+=LEDSTEP)
  {
    value = map(intensity, 0, INTENSITY_MAX, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    strip.setPixelColor(i, strip.ColorHSV(HUE_RED, 255, value));
  }
  strip.show();

  // stay on for random time
  delay(random(
      map(intensity, 0, INTENSITY_MAX, MAX_ON_X, MIN_ON_X),
      map(intensity, 0, INTENSITY_MAX, MAX_ON_Y, MIN_ON_Y)));

  // fade out and pause for random time
  if (intensity > 0)
  {
    int pauseTime = random(
      map(intensity, 0, INTENSITY_MAX, MAX_PAUSE_X, MIN_PAUSE_X),
      map(intensity, 0, INTENSITY_MAX, MAX_PAUSE_Y, MIN_PAUSE_Y));

    int fadeTime = 0;
    short value = map(intensity, 0, INTENSITY_MAX, MIN_BRIGHTNESS, MAX_BRIGHTNESS);

    while (value > 0 && fadeTime < pauseTime)
    {
      //Serial.print("fade: ");
      //Serial.println(value);

      for (byte i=0; i<NUMPIXELS; i+=LEDSTEP)
      {
        strip.setPixelColor(i, strip.ColorHSV(HUE_RED, 255, value));
      }

      strip.show();

      delay(20);

      fadeTime += 20;
      value -= map(intensity, 0, INTENSITY_MAX, MIN_FADE, MAX_FADE);
    }

    for (byte i=0; i<NUMPIXELS; i+=LEDSTEP)
    {
       strip.setPixelColor(i, 0);
    }
    strip.show();

    if (pauseTime - fadeTime > 0)
    {
      //Serial.println("start rest of pause");
      delay(pauseTime - fadeTime);
    }
  }
  }

  unsigned int IndexFromDate(byte day, byte month)
  {
  month--; day--;

  unsigned int idx = day;
  if (month > 0) day += 31;
  if (month > 1) day += 28;
  if (month > 2) day += 31;
  if (month > 3) day += 30;
  if (month > 4) day += 31;
  if (month > 5) day += 30;
  if (month > 6) day += 31;
  if (month > 7) day += 31;
  if (month > 8) day += 30;
  if (month > 9) day += 31;
  if (month > 10) day += 30;

  idx = constrain(idx, 0, 364);

  return idx;
  }

  void PrintValues ()
  {
  float maxT, minT;

  Serial.println("Number of entries of 30 year averages: ");
  Serial.println(sizeof(meansHist) / 4 + 1);
  Serial.println("***************************************");

  int steps = sizeof(means2018) / 4 + 1; // number of comparisons we want to make make
  // sizeof returns bytes. one float is 4 bytes: divide by 4 to get entries count.

  Serial.println("Number of entries of recent data: ");
  Serial.println(sizeof(steps));
  Serial.println("***************************************");

  int index = 0; //startDay; // index we start reading from meansHist from
  while (index < 365) { // step through temperature information

    float historicT = means2018[index]; // pgm_read_float(&means2018[index]);
    float recentT = meansHist[index]; // pgm_read_float(&meansHist[index]);

    float diff = recentT - historicT;

    Serial.print(index);
    Serial.print(": ");
    Serial.println(diff);

    maxT = max(diff, maxT);
    minT = min(diff, minT);

    index = index + 1;
    yield();
  }

  Serial.println("min & max differences");
  Serial.print(minT);
  Serial.print(", ");
  Serial.println(maxT);
  }
*/
