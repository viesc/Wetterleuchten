float findMaxDiff() {
  float maxD = 0;
  float maxIndex;
  for (int i = 0; i < TOTAL_DAYS; i++)  {
    float diff = abs(pgm_read_float(&diffT[i]));
    maxD = max(maxD, diff);
    if (maxD == diff) maxIndex = i;
  }
  Serial.println(String("Max temperature deviation in Celsius: ") + maxD);
  Serial.println(String("On ") +GetDayFromIndex(maxIndex) + String(".") + GetMonthFromIndex(maxIndex) + String(". / Index ") + maxIndex);
  return maxD;
}

float findMinDiff() {
  float minD = 0;
  for (int i = 0; i < TOTAL_DAYS; i++)  {
    float diff = pgm_read_float(&diffT[i]);
    minD = min(minD, diff);
  }
  return minD;
}

float GetTempDifference(unsigned short index)
{
  float value = pgm_read_float_near( diffT + index );
  //  Serial.println(value);
  return value;
}

int GetIntensityFromDifference (float difference)
{
  intensity = mapf2i(abs(difference), 0, maxDiff, 0, INTENSITY_MAX);
  return intensity;
}

// classic "map", but takes float values and outputs integer
int mapf2i(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (int)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

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
