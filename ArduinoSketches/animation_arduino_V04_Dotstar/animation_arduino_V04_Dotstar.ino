#include <Adafruit_DotStar.h>
#include <avr/pgmspace.h>

//#define DEBUG

// GENERAL SETTINGS
#define TOTAL_DAYS 365

// LED STRIP SETTINGS
#define DATAPIN_DOTSTAR1 4
#define CLOCKPIN 5
#define NUMPIXELS_DOTSTAR1 166  //144 per meter in this case
#define LEDSTEP 2 // use only every x LED in strip. mostly because electricity usage would be too high otherwise
#define LEDFRINGE_DOTSTAR1 3  // LEDS to not activate at end and beginning of strip (not working currently!)



// COLORS
#define GREEN_BRIGHT 50 // control brightness of green leds (tend to be brighter thats why i lower their brightness)

// TIME
#define DURATION_DAY 4931  //516 //1233 //2465 //4931 // 9863 // how long each day will be displayed in milliseconds

// FLICKER PARAMETERS

#define INTENSITY_MAX 100 // intensity boundary
#define CHANCEFLOOR 1000 // the higher the less likely to flicker
// minimum or maximum flicker time in milliseconds for low and high intensity of flickerung
#define LOW_FLICKER_MIN 40
#define LOW_FLICKER_MAX 100
#define HIGH_FLICKER_MIN 20
#define HIGH_FLICKER_MAX 600


// Global values for animation
int displayRainbow = false;
int intensity = 0; // global intensity variable
int currentDay = -1;
int startDay = 0;
bool isOn = false;
long tDay = 0; // timer for the day animations
long dayStart = 0; // stores start time of day animation in millis
float tempDiff = 0; // holds deviation temperature of day
float maxDiff; // min & max of temperature deviation, gets calculated in setup
float normal = 0; // +/- this amount counts as "normal" temperature
// Flicker globals
int flickerChance = 0;
long flickerStart;
int flickerTime;
bool ledsOn = true;
int mySeed = 255;
bool ledMix[NUMPIXELS_DOTSTAR1]; //stores which LEDs to color
bool ledDir[NUMPIXELS_DOTSTAR1]; //stores which LEDs to color


Adafruit_DotStar stripD1(NUMPIXELS_DOTSTAR1, DATAPIN_DOTSTAR1, CLOCKPIN, DOTSTAR_BGR);

// TEMPERATURE DATA
// historic data
//const float meansHist[] = {0.43, 0.60, 1.13, 1.01, 1.10, 0.54, 0.33, 0.21, -0.05, -0.10, -0.27, -0.14, -0.62, -0.27, 0.22, 0.22, 0.28, 0.83, 0.68, 0.90, 0.95, 0.37, 0.80, 0.72, 0.45, 0.48, 0.82, 1.07, 1.62, 1.17, 0.76, 1.00, 1.30, 1.93, 1.87, 1.67, 2.65, 3.30, 2.73, 2.43, 1.67, 1.75, 1.66, 1.87, 1.29, 1.13, 1.44, 1.50, 1.45, 2.15, 2.61, 2.41, 2.39, 2.24, 2.47, 2.64, 3.30, 2.96, 3.19, 3.28, 4.45, 3.96, 4.30, 4.29, 3.46, 3.89, 4.91, 5.13, 5.52, 5.85, 5.49, 5.71, 5.51, 5.61, 6.12, 6.81, 6.68, 7.14, 6.74, 5.78, 6.02, 6.58, 6.78, 7.77, 8.38, 8.17, 7.93, 8.45, 8.11, 8.47, 9.41, 9.90, 10.55, 10.13, 10.36, 9.59, 9.50, 9.76, 9.89, 9.95, 9.77, 1, 9.44, 9.41, 10.05, 10.35, 10.84, 10.65, 10.83, 10.88, 10.68, 11.51, 12.21, 12.55, 12.30, 12.91, 13.73, 13.65, 13.30, 13.70, 13.48, 14.69, 14.83, 14.63, 14.25, 14.68, 14.59, 14.75, 14.47, 15.08, 15.17, 15.86, 15.72, 15.53, 15.53, 16.14, 16.03, 16.39, 16.60, 16.63, 16.32, 16.32, 16.05, 15.55, 15.84, 16.71, 17.21, 18.09, 17.58, 16.94, 16.72, 16.69, 17.06, 17.37, 18.26, 18.18, 17.97, 18.43, 18.38, 18.13, 18.72, 19.05, 18.98, 18.59, 18.35, 19.19, 19.65, 18.25, 18.21, 18.14, 18.97, 19.44, 19.77, 19.51, 19.12, 18.80, 19.21, 19.84, 20.23, 20.22, 20.31, 20.38, 20.46, 20.36, 20.86, 21.17, 20.75, 20.68, 20.49, 20.06, 20.04, 20.43, 20.50, 20.37, 21.14, 20.73, 20.64, 21.53, 21.25, 20.92, 20.85, 20.91, 21.16, 21.49, 21.69, 21.89, 21.24, 21.06, 21.82, 21.51, 21.92, 22.51, 22.15, 22.15, 22.19, 22.02, 21.18, 21.11, 21.14, 20.65, 21.53, 21.56, 21.40, 21.29, 21.19, 21.16, 21.24, 21.44, 21.52, 21.69, 21.39, 21.33, 21.36, 20.94, 20.62, 20.40, 19.88, 19.91, 19.45, 19.78, 19.02, 18.57, 18.34, 18.21, 17.86, 17.56, 18.06, 17.78, 17.09, 17.27, 16.83, 16.91, 16.95, 16.64, 16.99, 17.16, 16.86, 16.44, 16.45, 16.10, 15.42, 15.60, 15.14, 15.87, 16.03, 15.99, 15.77, 15.37, 15.02, 15.13, 14.99, 14.27, 14.31, 14.24, 14.59, 14.42, 13.94, 13.32, 13.22, 13.67, 13.87, 13.09, 12.49, 12.38, 12.40, 12.21, 11.32, 11.37, 11.45, 11.02, 10.13, 9.67, 9.50, 9.80, 9.30, 9.09, 9.41, 9.23, 8.87, 8.63, 8.36, 8.87, 8.60, 8.46, 8.22, 8.51, 7.76, 7.59, 7.04, 7.00, 6.98, 7.15, 7.19, 6.54, 5.95, 5.29, 5.29, 5.03, 5.20, 5.52, 5.12, 5.58, 4.79, 4.41, 4.35, 3.60, 3.71, 3.84, 4.10, 4.58, 4.16, 4.56, 3.99, 3.45, 2.97, 2.24, 2.12, 2.26, 2.22, 2.00, 2.08, 1.80, 1.29, 0.74, 1.26, 2.05, 2.13, 1.46, 1.55, 1.41, 1.28, 1.04, 1.29, 1.84, 1.55, 0.94, 1.58, 1.82, 1.20, 0.68, 0.28, 0.36, 0.85, 1.31, 0.98};

// temperature deviaton from 30 years average
const float diffT[] PROGMEM = {4.9, 4.9, 3.4, 6.7, 6.9, 5.1, 7.0, 5.0, 6.4, 4.4, 5.1, 4.8, 1.9, 0.4, -1.4, 0.3, 3.4, 3.7, 3.9, 1.5, 0.6, -0.5, 3.2, 3.6, 1.9, 1.0, 1.9, 5.9, 10.0, 6.8, 3.2, 2.7, 3.0, 1.5, 0.2, -1.8, -3.0, -3.2, -1.5, -1.6, -1.9, 0.1, 1.7, -0.8, -0.2, 0.1, 0.4, -1.1, -0.2, -2.7, -3.0, -3.2, -3.0, -3.7, -5.8, -10.5, -12.6, -12.7, -12.7, -11.7, -10.3, -7.2, -6.8, -4.9, -2.9, 0.9, 1.8, 1.6, 3.4, 4.5, 5.9, 5.6, 2.5, 0.1, 2.0, -6.3, -9.9, -10.9, -8.0, -4.9, -4.1, -2.5, -1.3, -4.6, -4.2, -2.4, -3.0, 1.1, 0.7, 3.4, -0.7, -0.6, 0.9, 2.8, 3.2, 0.2, 0.2, 3.9, 8.2, 5.4, 5.2, 16.6, 8.0, 5.6, 6.7, 6.4, 6.2, 5.8, 7.1, 8.8, 7.9, 8.5, 8.1, 6.1, 7.6, 1.1, -0.0, 3.9, 8.4, 8.0, 4.8, 4.7, 5.1, 4.3, 3.6, 2.7, 3.8, 2.5, 2.7, 3.6, 3.5, 3.5, 4.2, 2.3, -3.0, -1.5, -1.6, 0.6, 0.6, 1.1, 1.3, 2.5, 4.9, 5.2, 1.3, 2.9, 4.3, 4.6, 5.4, 5.8, 6.8, 5.6, 4.9, 5.6, 4.3, 5.2, 4.0, 2.3, 4.9, 4.2, 5.0, 6.6, 4.6, 1.0, -1.5, 0.0, 2.4, 5.0, 5.4, 5.3, 5.8, 6.8, -3.9, -4.0, -2.8, -2.6, -1.7, -3.0, -2.4, 1.2, -0.2, -3.7, -2.9, -0.3, 1.9, 5.2, -0.3, 1.3, 1.0, 3.0, 0.4, -2.2, -0.4, 1.4, 1.6, 2.8, 1.6, -0.3, 1.5, 3.4, 2.6, 1.4, -1.2, -0.1, 3.3, 2.3, 1.8, 3.1, 3.2, 5.4, 5.4, 4.9, 3.6, 2.8, 4.6, 4.6, 5.5, 4.3, 3.9, 6.2, 6.9, 3.2, -0.7, 2.3, 3.8, 1.1, 1.8, 1.1, 1.7, 2.7, 3.4, 5.4, 5.1, 4.8, 5.4, 2.8, -2.1, -4.8, -2.6, 0.5, 2.5, 2.5, 0.8, -0.9, -1.0, 0.4, 0.7, 3.3, 2.1, 2.0, 3.6, 1.7, 2.8, 6.4, 5.1, 4.5, 2.4, 2.1, 2.1, 3.3, 5.6, 4.8, 5.7, 5.4, -0.5, -0.6, -3.3, -5.5, -5.3, -1.9, 1.0, -1.9, -3.7, -3.8, -4.5, -0.7, -1.1, -0.3, 3.3, 3.6, 1.5, 1.1, 3.6, 3.5, 6.5, 2.9, 4.8, 2.9, 3.0, 2.6, 4.0, 4.1, 3.5, 0.0, 0.9, 2.3, 0.7, 4.3, 4.1, 0.6, 1.8, 5.6, 7.9, 2.9, 6.9, 5.3, 4.7, 4.3, 4.3, 5.8, 4.6, 3.7, 3.5, 3.0, 2.9, 2.8, 4.5, 4.7, 0.3, -1.3, -2.8, -4.3, -4.1, -2.0, 0.3, 0.1, 2.4, 3.1, 1.2, 0.4, -3.2, -4.3, -6.3, -5.2, -4.8, -3.1, 0.7, 6.2, 0.8, -0.5, 2.6, 5.5, 5.7, 5.1, 1.7, -0.5, -2.8, -2.8, -2.5, -2.3, -1.6, 1.7, -0.6, -3.2, -1.6, 7.5, 7.0, 2.9, 2.0, 3.4, 5.6, 5.8, 2.7, 3.0, 4.1};
// normal deviation from average temp per month. manually collected from
float normalD[] = {5.5, 5.0, 5.5, 4.0, 3.0, 3.0, 3.0, 3.0, 2.5, 3.5, 4.0, 5.0};


void setup()
{
  Serial.begin(9600);

  stripD1.begin();
  stripD1.show();
  //  minDiff = findMinDiff();

  Serial.println("Hej!");
  Serial.println("** Climate Justice now! **");
  Serial.println(String("Power drain on current LED settings if just using r, g, or b: ") + (NUMPIXELS_DOTSTAR1 * 0.02 / LEDSTEP) + String("A"));
  //  Serial.println(String("Max possible current drain on dotstar: ") + (NUMPIXELS_DOTSTAR1 * 0.06) + String("A"));
  maxDiff = findMaxDiff();
  Serial.print("Max temperature deviation in Celsius: "); Serial.println(maxDiff);
  //  testLED(NUMPIXELS_DOTSTAR1);
}

void loop()
{
  //  randomSeed(analogRead(A0)); // change random based on
  //  if (currentDay + 1 == 0) testLED(NUMPIXELS_DOTSTAR1);

  // Rainbow animation (once per year)
  if (currentDay + 1 == 0) {
    if (displayRainbow) {
      Serial.println("\nRainbow time.");
      rainbowFade2White(2, 2, 0);
      Serial.println("Animation will start soon.");
      delay(500); // Pause 1/2 second
    }
    currentDay += startDay;
  }


  if (millis() - dayStart >= DURATION_DAY || currentDay < 0)
  {
    //mySeed = random(1024);

    //    tDay = 0;
    dayStart = millis();
    // Modulo operator makes value of currentDay stay bewtween 0 and TOTAL_DAYS
    // This actually generates the loop through days of the year!
    currentDay = (currentDay + 1) % TOTAL_DAYS;
    Serial.println();
    Serial.println(String("current day: ") + currentDay + String(", ")
                   + GetDayFromIndex(currentDay) + String(".") + GetMonthFromIndex(currentDay) + String("."));

    tempDiff = GetTempDifference(currentDay);
    Serial.println(String("current temp deviation: ") + tempDiff);
    normal = normalD[GetMonthFromIndex(currentDay) - 1] * 0.8;
    Serial.println(String("normal deviation: ") + normal);

    intensity = GetIntensityFromDifference(tempDiff);
    Serial.println(String("current intensity: ") + intensity);

    // subtract normal deviation from daily deviation, only start flickering when too warm or cold for season
    float abnormalD = abs(tempDiff) - normal;
    //  Serial.print(abs(tempDiff)); Serial.print("-");Serial.println(normalD[GetMonthFromIndex(currentDay) - 1]);
    flickerChance = max(mapf2i(abnormalD, 0, maxDiff - normal, 0, CHANCEFLOOR), 0);

    Serial.println(String("flicker chance: ") + flickerChance);

    mixLedArrayEven();

    if (tempDiff > 0) {
      Serial.println("*mixing in hot pixels");
      mixInHeat(NUMPIXELS_DOTSTAR1, LEDFRINGE_DOTSTAR1, ledMix);
    }
    else {
      Serial.println("*mixing in cold pixels");
      mixInCold(NUMPIXELS_DOTSTAR1, LEDFRINGE_DOTSTAR1, ledMix);
    }
  } // else if (millis()%1000 == 0) Serial.print(".");

  if (ledsOn) { // if leds are on check if flickering should be activated
    // the higher intensity the higher the chance of flicker
    if (random(300000) < flickerChance) { // max chance = 1000 in 5000 aka every 5th loop
      flickerStart = millis();
      turnOff();
      ledsOn = false;
      int minFlicker = map(flickerChance, 0, 1000, LOW_FLICKER_MIN, LOW_FLICKER_MAX); // define min possible flicker time
      int maxFlicker = map(flickerChance, 0, 1000, HIGH_FLICKER_MIN, HIGH_FLICKER_MAX); // define max possible flicker time
      flickerTime = random(minFlicker, maxFlicker); // set random flicker time
      //      Serial.println(String("min, max flicker time: ") + minFlicker + ", " + maxFlicker);
      //      Serial.println(String("flick on for: ") + flickerTime);
    }
  } else { // if leds are off, check when to turn on again
    if (millis() - flickerStart > flickerTime)   // current ms - start ms = time passed, check if time passed is > flickerTime
    {
      ledsOn = true;
      if (tempDiff > 0) mixInHeat(NUMPIXELS_DOTSTAR1, LEDFRINGE_DOTSTAR1, ledMix);
      else mixInCold(NUMPIXELS_DOTSTAR1, LEDFRINGE_DOTSTAR1, ledMix);
    }
  }


  //  tDay = tDay + LOOP_CLOCK;

  // delay(LOOP_CLOCK);
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
