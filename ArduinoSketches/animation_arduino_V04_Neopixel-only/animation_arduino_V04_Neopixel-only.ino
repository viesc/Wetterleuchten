#include <Adafruit_NeoPixel.h>
//#include <Adafruit_DotStar.h>
#include <avr/pgmspace.h>

//#define DEBUG

// GENERAL SETTINGS
#define TOTAL_DAYS 365


// LED Strip stuff
#define DATAPIN_NEOPIX1 4
#define NUMPIXELS_NEOPIX1 144 // 144
#define LEDFRINGE_NEOPIX1 0

//#define DATAPIN_DOTSTAR1 14
//#define NUMPIXELS_DOTSTAR1 166 // 166
//#define LEDFRINGE_DOTSTAR1 9
//
//#define DATAPIN_DOTSTAR2 15
//#define NUMPIXELS_DOTSTAR2 166 // 166
//#define LEDFRINGE_DOTSTAR2 9

//#define CLOCKPIN 12

#define LEDSTEP 2 // use only every x LED in strip

// COLORS
#define HUE_RED 2000
#define HUE_BLUE 43690
#define HUE_GREY 46000
#define HUE_YELLOW 15000

// ANIMATION PARAMETERS
#define DURATION_DAY 2465 //1233 //2465 //4931 // 9863 // ms duration for the data of one day
#define LOOP_CLOCK 20

// intensity boundary
#define INTENSITY_MAX 100

// change values to define behavior on minimum or maximum intensity, interpolates in between
#define MIN_BRIGHTNESS 30
#define MAX_BRIGHTNESS 255
#define MIN_FLICKER_ON 10
#define MAX_FLICKER_ON 300
#define MIN_FLICKER_OFF 30
#define MAX_FLICKER_OFF 5
#define MIN_FADE 150
#define MAX_FADE 150

// Global values for animation
int intensity;
int currentDay = -1;
int hue;
int value = 255;
int saturation;
bool isOn = false;
long tDay = 0; // timer for the day animations
float tempDiff = 0; // holds deviation temperature of day
float minDiff, maxDiff; // min & max of temperature deviation
const float normal = 0; // +/- this amount counts as "normal" temperature
// Flicker globals
long flickerStart;
int flickerTime;
bool ledsOn = true;
int mySeed = 255;

//Adafruit_DotStar stripD1(NUMPIXELS_DOTSTAR1, DATAPIN_DOTSTAR1, CLOCKPIN, DOTSTAR_BGR);
Adafruit_NeoPixel stripN1(NUMPIXELS_NEOPIX1, DATAPIN_NEOPIX1, NEO_GRBW + NEO_KHZ800);

// TEMPERATURE DATA
// historic data
//const float meansHist[] = {0.43, 0.60, 1.13, 1.01, 1.10, 0.54, 0.33, 0.21, -0.05, -0.10, -0.27, -0.14, -0.62, -0.27, 0.22, 0.22, 0.28, 0.83, 0.68, 0.90, 0.95, 0.37, 0.80, 0.72, 0.45, 0.48, 0.82, 1.07, 1.62, 1.17, 0.76, 1.00, 1.30, 1.93, 1.87, 1.67, 2.65, 3.30, 2.73, 2.43, 1.67, 1.75, 1.66, 1.87, 1.29, 1.13, 1.44, 1.50, 1.45, 2.15, 2.61, 2.41, 2.39, 2.24, 2.47, 2.64, 3.30, 2.96, 3.19, 3.28, 4.45, 3.96, 4.30, 4.29, 3.46, 3.89, 4.91, 5.13, 5.52, 5.85, 5.49, 5.71, 5.51, 5.61, 6.12, 6.81, 6.68, 7.14, 6.74, 5.78, 6.02, 6.58, 6.78, 7.77, 8.38, 8.17, 7.93, 8.45, 8.11, 8.47, 9.41, 9.90, 10.55, 10.13, 10.36, 9.59, 9.50, 9.76, 9.89, 9.95, 9.77, 1, 9.44, 9.41, 10.05, 10.35, 10.84, 10.65, 10.83, 10.88, 10.68, 11.51, 12.21, 12.55, 12.30, 12.91, 13.73, 13.65, 13.30, 13.70, 13.48, 14.69, 14.83, 14.63, 14.25, 14.68, 14.59, 14.75, 14.47, 15.08, 15.17, 15.86, 15.72, 15.53, 15.53, 16.14, 16.03, 16.39, 16.60, 16.63, 16.32, 16.32, 16.05, 15.55, 15.84, 16.71, 17.21, 18.09, 17.58, 16.94, 16.72, 16.69, 17.06, 17.37, 18.26, 18.18, 17.97, 18.43, 18.38, 18.13, 18.72, 19.05, 18.98, 18.59, 18.35, 19.19, 19.65, 18.25, 18.21, 18.14, 18.97, 19.44, 19.77, 19.51, 19.12, 18.80, 19.21, 19.84, 20.23, 20.22, 20.31, 20.38, 20.46, 20.36, 20.86, 21.17, 20.75, 20.68, 20.49, 20.06, 20.04, 20.43, 20.50, 20.37, 21.14, 20.73, 20.64, 21.53, 21.25, 20.92, 20.85, 20.91, 21.16, 21.49, 21.69, 21.89, 21.24, 21.06, 21.82, 21.51, 21.92, 22.51, 22.15, 22.15, 22.19, 22.02, 21.18, 21.11, 21.14, 20.65, 21.53, 21.56, 21.40, 21.29, 21.19, 21.16, 21.24, 21.44, 21.52, 21.69, 21.39, 21.33, 21.36, 20.94, 20.62, 20.40, 19.88, 19.91, 19.45, 19.78, 19.02, 18.57, 18.34, 18.21, 17.86, 17.56, 18.06, 17.78, 17.09, 17.27, 16.83, 16.91, 16.95, 16.64, 16.99, 17.16, 16.86, 16.44, 16.45, 16.10, 15.42, 15.60, 15.14, 15.87, 16.03, 15.99, 15.77, 15.37, 15.02, 15.13, 14.99, 14.27, 14.31, 14.24, 14.59, 14.42, 13.94, 13.32, 13.22, 13.67, 13.87, 13.09, 12.49, 12.38, 12.40, 12.21, 11.32, 11.37, 11.45, 11.02, 10.13, 9.67, 9.50, 9.80, 9.30, 9.09, 9.41, 9.23, 8.87, 8.63, 8.36, 8.87, 8.60, 8.46, 8.22, 8.51, 7.76, 7.59, 7.04, 7.00, 6.98, 7.15, 7.19, 6.54, 5.95, 5.29, 5.29, 5.03, 5.20, 5.52, 5.12, 5.58, 4.79, 4.41, 4.35, 3.60, 3.71, 3.84, 4.10, 4.58, 4.16, 4.56, 3.99, 3.45, 2.97, 2.24, 2.12, 2.26, 2.22, 2.00, 2.08, 1.80, 1.29, 0.74, 1.26, 2.05, 2.13, 1.46, 1.55, 1.41, 1.28, 1.04, 1.29, 1.84, 1.55, 0.94, 1.58, 1.82, 1.20, 0.68, 0.28, 0.36, 0.85, 1.31, 0.98};

// 2018 data
//const float means2018[] = {5.3, 5.5, 4.5, 7.7, 8.0, 5.6, 7.3, 5.2, 6.3, 4.3, 4.8, 4.7, 1.3, 0.1, -1.2, 0.5, 3.7, 4.5, 4.6, 2.4, 1.5, -0.1, 4.0, 4.3, 2.3, 1.5, 2.7, 7.0, 11.6, 8.0, 4.0, 3.7, 4.3, 3.4, 2.1, -0.1, -0.3, 0.1, 1.2, 0.8, -0.2, 1.8, 3.4, 1.1, 1.1, 1.2, 1.8, 0.4, 1.3, -0.5, -0.4, -0.8, -0.6, -1.5, -3.3, -7.9, -9.3, -9.7, -9.5, -8.4, -5.8, -3.2, -2.5, -0.6, 0.6, 4.8, 6.7, 6.7, 8.9, 10.3, 11.4, 11.3, 8.0, 5.7, 8.1, 0.5, -3.2, -3.8, -1.3, 0.9, 1.9, 4.1, 5.5, 3.2, 4.2, 5.8, 4.9, 9.5, 8.8, 11.9, 8.7, 9.3, 11.5, 12.9, 13.6, 9.8, 9.7, 13.7, 18.1, 15.3, 15.0, 17.6, 17.4, 15.0, 16.7, 16.7, 17.0, 16.4, 17.9, 19.7, 18.6, 20.0, 20.3, 18.6, 19.9, 14.0, 13.7, 17.5, 21.7, 21.7, 18.3, 19.4, 19.9, 18.9, 17.8, 17.4, 18.4, 17.2, 17.2, 18.7, 18.7, 19.4, 19.9, 17.8, 12.5, 14.6, 14.4, 17.0, 17.2, 17.7, 17.6, 18.8, 20.9, 20.7, 17.1, 19.6, 21.5, 22.7, 23.0, 22.7, 23.5, 22.3, 22.0, 23.0, 22.6, 23.4, 22.0, 20.7, 23.3, 22.3, 23.7, 25.6, 23.6, 19.6, 16.9, 19.2, 22.0, 23.2, 23.6, 23.4, 24.8, 26.2, 15.9, 15.5, 16.3, 16.2, 17.5, 16.8, 17.8, 21.4, 20.1, 16.7, 17.6, 20.1, 22.8, 26.4, 20.5, 22.0, 21.5, 23.1, 20.4, 18.2, 20.1, 21.8, 22.7, 23.5, 22.2, 21.2, 22.7, 24.3, 23.4, 22.3, 20.0, 21.4, 25.0, 24.2, 23.0, 24.2, 25.0, 26.9, 27.3, 27.4, 25.7, 24.9, 26.8, 26.6, 26.7, 25.4, 25.0, 26.8, 28.4, 24.8, 20.7, 23.6, 25.0, 22.3, 23.0, 22.5, 23.2, 24.4, 24.8, 26.7, 26.5, 25.7, 26.0, 23.2, 17.8, 15.1, 16.9, 20.3, 21.5, 21.1, 19.1, 17.3, 16.9, 18.0, 18.8, 21.1, 19.2, 19.3, 20.4, 18.6, 19.7, 23.0, 22.1, 21.7, 19.3, 18.5, 18.5, 19.4, 21.0, 20.4, 20.8, 21.3, 15.5, 15.4, 12.5, 9.9, 9.7, 13.2, 16.0, 12.4, 10.6, 10.4, 10.1, 13.7, 12.8, 13.0, 16.5, 17.3, 15.4, 14.2, 16.1, 15.9, 18.9, 15.1, 16.1, 14.3, 14.4, 13.6, 14.1, 13.8, 13.0, 9.8, 10.2, 11.4, 10.1, 13.5, 13.0, 9.2, 10.2, 14.5, 16.5, 11.4, 15.1, 13.8, 12.5, 11.9, 11.3, 12.8, 11.6, 10.8, 10.7, 9.5, 8.8, 8.1, 9.8, 9.7, 5.5, 4.2, 2.3, 1.3, 0.7, 2.4, 4.6, 3.7, 6.1, 6.9, 5.3, 5.0, 1.0, 0.3, -2.3, -1.7, -1.8, -0.9, 2.8, 8.5, 3.0, 1.5, 4.7, 7.3, 7.0, 5.8, 3.0, 1.6, -0.7, -1.3, -0.9, -0.9, -0.3, 2.7, 0.7, -1.4, -0.0, 8.4, 8.6, 4.7, 3.2, 4.1, 5.9, 6.2, 3.5, 4.3, 5.1};

const float diffT[] PROGMEM = {4.9, 4.9, 3.4, 6.7, 6.9, 5.1, 7.0, 5.0, 6.4, 4.4, 5.1, 4.8, 1.9, 0.4, -1.4, 0.3, 3.4, 3.7, 3.9, 1.5, 0.6, -0.5, 3.2, 3.6, 1.9, 1.0, 1.9, 5.9, 10.0, 6.8, 3.2, 2.7, 3.0, 1.5, 0.2, -1.8, -3.0, -3.2, -1.5, -1.6, -1.9, 0.1, 1.7, -0.8, -0.2, 0.1, 0.4, -1.1, -0.2, -2.7, -3.0, -3.2, -3.0, -3.7, -5.8, -10.5, -12.6, -12.7, -12.7, -11.7, -10.3, -7.2, -6.8, -4.9, -2.9, 0.9, 1.8, 1.6, 3.4, 4.5, 5.9, 5.6, 2.5, 0.1, 2.0, -6.3, -9.9, -10.9, -8.0, -4.9, -4.1, -2.5, -1.3, -4.6, -4.2, -2.4, -3.0, 1.1, 0.7, 3.4, -0.7, -0.6, 0.9, 2.8, 3.2, 0.2, 0.2, 3.9, 8.2, 5.4, 5.2, 16.6, 8.0, 5.6, 6.7, 6.4, 6.2, 5.8, 7.1, 8.8, 7.9, 8.5, 8.1, 6.1, 7.6, 1.1, -0.0, 3.9, 8.4, 8.0, 4.8, 4.7, 5.1, 4.3, 3.6, 2.7, 3.8, 2.5, 2.7, 3.6, 3.5, 3.5, 4.2, 2.3, -3.0, -1.5, -1.6, 0.6, 0.6, 1.1, 1.3, 2.5, 4.9, 5.2, 1.3, 2.9, 4.3, 4.6, 5.4, 5.8, 6.8, 5.6, 4.9, 5.6, 4.3, 5.2, 4.0, 2.3, 4.9, 4.2, 5.0, 6.6, 4.6, 1.0, -1.5, 0.0, 2.4, 5.0, 5.4, 5.3, 5.8, 6.8, -3.9, -4.0, -2.8, -2.6, -1.7, -3.0, -2.4, 1.2, -0.2, -3.7, -2.9, -0.3, 1.9, 5.2, -0.3, 1.3, 1.0, 3.0, 0.4, -2.2, -0.4, 1.4, 1.6, 2.8, 1.6, -0.3, 1.5, 3.4, 2.6, 1.4, -1.2, -0.1, 3.3, 2.3, 1.8, 3.1, 3.2, 5.4, 5.4, 4.9, 3.6, 2.8, 4.6, 4.6, 5.5, 4.3, 3.9, 6.2, 6.9, 3.2, -0.7, 2.3, 3.8, 1.1, 1.8, 1.1, 1.7, 2.7, 3.4, 5.4, 5.1, 4.8, 5.4, 2.8, -2.1, -4.8, -2.6, 0.5, 2.5, 2.5, 0.8, -0.9, -1.0, 0.4, 0.7, 3.3, 2.1, 2.0, 3.6, 1.7, 2.8, 6.4, 5.1, 4.5, 2.4, 2.1, 2.1, 3.3, 5.6, 4.8, 5.7, 5.4, -0.5, -0.6, -3.3, -5.5, -5.3, -1.9, 1.0, -1.9, -3.7, -3.8, -4.5, -0.7, -1.1, -0.3, 3.3, 3.6, 1.5, 1.1, 3.6, 3.5, 6.5, 2.9, 4.8, 2.9, 3.0, 2.6, 4.0, 4.1, 3.5, 0.0, 0.9, 2.3, 0.7, 4.3, 4.1, 0.6, 1.8, 5.6, 7.9, 2.9, 6.9, 5.3, 4.7, 4.3, 4.3, 5.8, 4.6, 3.7, 3.5, 3.0, 2.9, 2.8, 4.5, 4.7, 0.3, -1.3, -2.8, -4.3, -4.1, -2.0, 0.3, 0.1, 2.4, 3.1, 1.2, 0.4, -3.2, -4.3, -6.3, -5.2, -4.8, -3.1, 0.7, 6.2, 0.8, -0.5, 2.6, 5.5, 5.7, 5.1, 1.7, -0.5, -2.8, -2.8, -2.5, -2.3, -1.6, 1.7, -0.6, -3.2, -1.6, 7.5, 7.0, 2.9, 2.0, 3.4, 5.6, 5.8, 2.7, 3.0, 4.1};

void setup()
{
  Serial.begin(9600);

  //  stripD1.begin();
  //  stripD1.show();

  stripN1.begin();
  stripN1.show();
  testLED(NUMPIXELS_NEOPIX1);
  delay(200);
  turnOff();
  delay(200);
  fillWhite(255);
  delay(500);
  turnOff();
  delay(200);
  mixInHeat(50, NUMPIXELS_NEOPIX1, LEDFRINGE_NEOPIX1);
  delay(500);
  turnOff();
  delay(200);
  mixInCold(50, NUMPIXELS_NEOPIX1, LEDFRINGE_NEOPIX1);
  delay(500);
  turnOff();
  delay(2000);

  //Serial.println(String("\npower drain on current LED settings: ") + (NUMPIXELS * 0.02 / LEDSTEP) + String("A"));
  maxDiff = findMaxDiff();
  //  minDiff = findMinDiff();

  Serial.println("Hej!");
  Serial.println("** SETTINGS **");
  Serial.print("normal range +/- "); Serial.println(normal);
  Serial.print("max deviation found: "); Serial.println(maxDiff);
}

void loop()
{
//  randomSeed(mySeed);

  if (tDay >= DURATION_DAY || currentDay < 0)
  {
    //mySeed = random(1024);
    tDay = 0;
    currentDay = (currentDay + 1) % TOTAL_DAYS;

    Serial.println();
    Serial.println(String("current day: ") + currentDay + String(", ")
                   + GetDayFromIndex(currentDay) + String(".") + GetMonthFromIndex(currentDay) + String("."));

    tempDiff = GetTempDifference(currentDay);
    short intensity = GetIntensityFromDifference(tempDiff);
    Serial.println(String("current intensity: ") + intensity);

    Serial.println(String("current temp diff: ") + tempDiff);

    //short intensity = GetIntensityFromPoti();

    int curHue, curSat;

    if (intensity == 0)
    {
      curHue = HUE_GREY;
    }
    else if (intensity < 0)
    {
      curHue = HUE_BLUE;
    }
    else
    {
      curHue = HUE_RED;
    }

    curSat = (intensity == 0) ? 0 : (int)((abs(intensity) / (float)INTENSITY_MAX) * 55) + 200;

    //fillWhite(intensity);
    if (tempDiff > 0) mixInHeat(intensity, NUMPIXELS_NEOPIX1, LEDFRINGE_NEOPIX1);
    else mixInCold(intensity, NUMPIXELS_NEOPIX1, LEDFRINGE_NEOPIX1);

    //UpdateLEDs(NUMPIXELS_NEOPIX1, LEDFRINGE_NEOPIX1, intensity, curHue, curSat, stripN1);
    //UpdateLEDs(true, NUMPIXELS_DOTSTAR1, LEDFRINGE_DOTSTAR1, intensity, curHue, curSat, stripD1, stripN1);

    //EncodeDate();
  }

  if (ledsOn) { // if leds are on check if flickering should be activated
    if (random(100) < intensity/3) { // the higher intensity the higher the chance of flicker
           
      flickerStart = millis();
      turnOff();
      ledsOn = false;
      flickerTime = random(MIN_FLICKER_ON, MAX_FLICKER_ON); // set random flicker time
#ifdef DEBUG
      Serial.println("flick on");
#endif
    }
  } else { // if leds are off, check when to turn on again
     flickerTime = random(MIN_FLICKER_ON, MAX_FLICKER_ON); // set random flicker time
    if (millis() - flickerStart > flickerTime)   // current ms - start ms = time passed, check if time passed is > flickerTime
    {
      ledsOn = true;
      if (tempDiff > 0) mixInHeat(intensity, NUMPIXELS_NEOPIX1, LEDFRINGE_NEOPIX1);
      else mixInCold(intensity, NUMPIXELS_NEOPIX1, LEDFRINGE_NEOPIX1);
#ifdef DEBUG
      Serial.println("flick off");
#endif
    }
  }


  tDay = tDay + LOOP_CLOCK;

  delay(LOOP_CLOCK);
}

float GetTempDifference(unsigned short index)
{
  float value = pgm_read_float_near( diffT + index );
  //  Serial.println(value);
  return value;
}


int GetIntensityFromDifference (float difference)
{
  intensity = mapf2i(abs(difference), normal, maxDiff, 0, INTENSITY_MAX);
  return intensity;
}
