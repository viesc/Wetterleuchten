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
#define LEDFRINGE_DOTSTAR1 4  // LEDS to not activate at end and beginning of strip (not working currently!)




// COLORS
#define GREEN_BRIGHT 50 // control brightness of green leds (tend to be brighter thats why i lower their brightness)

// TIME
#define DURATION_DAY 9863  //516 //1233 //2465 //4931 // 9863 // how long each day will be displayed in milliseconds

// FLICKER PARAMETERS

#define INTENSITY_MAX 100 // intensity boundary
#define CHANCEFLOOR 1000 // the lower the less likely to flicker
// minimum or maximum flicker time in milliseconds for low and high intensity of flickering
#define MIN_FLICKER_LOW 80
#define MAX_FLICKER_LOW 20  // faster flickers when high intensity
#define MIN_FLICKER_HIGH 200
#define MAX_FLICKER_HIGH 1000 // but also slower flickers possible
// this is how it works below, kinda quirky
//int minFlicker = map(intensity, 0, INTENSITY_MAX, MIN_FLICKER_LOW, MAX_FLICKER_LOW); // define min possible flicker time
//int maxFlicker = map(intensity, 0, INTENSITY_MAX, MIN_FLICKER_HIGH, MAX_FLICKER_HIGH); // define max possible flicker time
//flickerTime = random(minFlicker, maxFlicker); // set random flicker time


// Global values for animation
int displayRainbow = false;
int intensity = 0; // global intensity variable
int currentDay = -1;
int startDay = 96;  // day of the year to start loop
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
int ledMix[NUMPIXELS_DOTSTAR1]; //stores which LEDs to color

Adafruit_DotStar stripD1(NUMPIXELS_DOTSTAR1, DATAPIN_DOTSTAR1, CLOCKPIN, DOTSTAR_BGR);

// TEMPERATURE DATA

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
  //  testLED(NUMPIXELS_DOTSTAR1);
}

void loop()
{
  //  randomSeed(analogRead(A0)); // change random based on
  //  if (currentDay + 1 == 0) testLED(NUMPIXELS_DOTSTAR1);

  int ledFringe = LEDFRINGE_DOTSTAR1;

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

    mixLedArrayEven(ledFringe);

    if (tempDiff > 0) {
      Serial.println("displaying *hot* pixels");
      displayTemperature(1);
    }
    else {
      Serial.println("displaying *cold* pixels");
      displayTemperature(0);
    }
  } // else if (millis()%1000 == 0) Serial.print(".");

  if (ledsOn && millis() - flickerStart > flickerTime/2) { // if leds and rest of flickertime has passed: check if flickering should be activated
    if (random(300000) < flickerChance) {
      flickerStart = millis();
      turnOff();
      ledsOn = false;
      int minFlicker = map(intensity, 0, INTENSITY_MAX, MIN_FLICKER_LOW, MAX_FLICKER_LOW); // define min possible flicker time
      int maxFlicker = map(intensity, 0, INTENSITY_MAX, MIN_FLICKER_HIGH, MAX_FLICKER_HIGH); // define max possible flicker time
      flickerTime = random(minFlicker, maxFlicker); // set random flicker time
      Serial.println(String("min, max flicker time: ") + minFlicker + ", " + maxFlicker);
      Serial.println(String("flick on for: ") + flickerTime);
    }
  } else { // if leds are off, check when to turn on again
    if (millis() - flickerStart > flickerTime/2)   // current ms - start ms = time passed, check if time passed is > flickerTime/2
    {
      ledsOn = true;
      if (tempDiff > 0) displayTemperature(1);
      else displayTemperature(0);
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
