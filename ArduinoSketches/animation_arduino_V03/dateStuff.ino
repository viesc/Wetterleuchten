short GetMonthFromIndex(short idx)
{
  unsigned int month = 0;
  if (idx >= 31) {
    month++;
    idx -= 31;
  }
  if (idx >= 28) {
    month++;
    idx -= 28;
  }
  if (idx >= 31) {
    month++;
    idx -= 31;
  }
  if (idx >= 30) {
    month++;
    idx -= 30;
  }
  if (idx >= 31) {
    month++;
    idx -= 31;
  }
  if (idx >= 30) {
    month++;
    idx -= 30;
  }
  if (idx >= 31) {
    month++;
    idx -= 31;
  }
  if (idx >= 31) {
    month++;
    idx -= 31;
  }
  if (idx >= 30) {
    month++;
    idx -= 30;
  }
  if (idx >= 31) {
    month++;
    idx -= 31;
  }
  if (idx >= 30) {
    month++;
    idx -= 30;
  }

  return (month + 1);
}

short GetDayFromIndex(short idx)
{
  if (idx >= 31) {
    idx -= 31;
  }
  if (idx >= 28) {
    idx -= 28;
  }
  if (idx >= 31) {
    idx -= 31;
  }
  if (idx >= 30) {
    idx -= 30;
  }
  if (idx >= 31) {
    idx -= 31;
  }
  if (idx >= 30) {
    idx -= 30;
  }
  if (idx >= 31) {
    idx -= 31;
  }
  if (idx >= 31) {
    idx -= 31;
  }
  if (idx >= 30) {
    idx -= 30;
  }
  if (idx >= 31) {
    idx -= 31;
  }
  if (idx >= 30) {
    idx -= 30;
  }

  return (idx + 1);
}

//void EncodeDateD(Adafruit_DotStar strip)
//{
//  int month = GetMonthFromIndex(currentDay);
//  int day = GetDayFromIndex(currentDay);
//  Serial.println(month);
//  Serial.println(day);
//
//  int shift = 25;
//
//  // dd
//  strip.setPixelColor(10 + shift, stripD1.ColorHSV(HUE_RED, 255, (1 & day == 1) ? 255 : 20));
//  strip.setPixelColor(20 + shift, strip.ColorHSV(HUE_RED, 255, (2 & day == 2) ? 255 : 20));
//  strip.setPixelColor(30 + shift, strip.ColorHSV(HUE_RED, 255, (4 & day == 4) ? 255 : 20));
//  strip.setPixelColor(40 + shift, strip.ColorHSV(HUE_RED, 255, (8 & day == 8) ? 255 : 20));
//  strip.setPixelColor(50 + shift, strip.ColorHSV(HUE_RED, 255, (16 & day == 16) ? 255 : 20));
//
//  // mm
//  strip.setPixelColor(60 + shift, strip.ColorHSV(HUE_BLUE, 255, (1 & month == 1) ? 255 : 20));
//  strip.setPixelColor(70 + shift, strip.ColorHSV(HUE_BLUE, 255, (2 & month == 2) ? 255 : 20));
//  strip.setPixelColor(80 + shift, strip.ColorHSV(HUE_BLUE, 255, (4 & month == 4) ? 255 : 20));
//  strip.setPixelColor(90 + shift, strip.ColorHSV(HUE_BLUE, 255, (8 & month == 8) ? 255 : 20));
//
//  strip.show();
//}
