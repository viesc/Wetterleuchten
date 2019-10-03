float findMaxDiff() {
  float maxD = 0;
  for (int i = 0; i < TOTAL_DAYS; i++)  {
    float diff = abs(pgm_read_float(&diffT[i]));
    maxD = max(maxD, diff);
  }
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
