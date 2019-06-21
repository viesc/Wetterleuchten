#include <Adafruit_DotStar.h>
#include <SPI.h> 

#define DATAPIN 14
#define CLOCKPIN 12

#define NUMPIXELS 166 // 166
#define LEDSTEP 1 // use only every x LED in strip

#define HUE_RED 0
#define HUE_BLUE 43690

// ANIMATION PARAMETERS
// intensity boundaries
#define INTENSITY_MIN 0
#define INTENSITY_MAX 100

// change values to define behavior on minimum or maximum intensity, interpolates in between
#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 255
#define MIN_PAUSE_X 0
#define MAX_PAUSE_X 200
#define MIN_PAUSE_Y 0
#define MAX_PAUSE_Y 4000
#define MIN_ON_X 40
#define MAX_ON_X 1000
#define MIN_ON_Y 500
#define MAX_ON_Y 3000
#define MIN_FADE 2
#define MAX_FADE 20

short intensity = 0; // set actual intensity, 0 = steady, 100 = max flickering

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

void setup() 
{
  Serial.begin(9600);
  
  strip.begin();
  strip.show(); 
}

void loop() 
{  
  // TEMP GET INTENSIY FROM POTI
  intensity = constrain((float)analogRead(A0)/650 * INTENSITY_MAX, 0, INTENSITY_MAX);
  Serial.println(intensity);
  
  // turn on
  for (byte i=0; i<NUMPIXELS; i+=LEDSTEP)
  {
    strip.setPixelColor(i, strip.ColorHSV(HUE_RED, 255, map(intensity, INTENSITY_MIN, INTENSITY_MAX, MIN_BRIGHTNESS, MAX_BRIGHTNESS)));
  }
  strip.show();

  // stay on for random time
  delay(random(
      map(intensity, INTENSITY_MIN, INTENSITY_MAX, MAX_ON_X, MIN_ON_X),
      map(intensity, INTENSITY_MIN, INTENSITY_MAX, MAX_ON_Y, MIN_ON_Y)));

  // fade out and pause for random time
  if (intensity > 0)
  {
    int pauseTime = random(
      map(intensity, INTENSITY_MIN, INTENSITY_MAX, MAX_PAUSE_X, MIN_PAUSE_X),
      map(intensity, INTENSITY_MIN, INTENSITY_MAX, MAX_PAUSE_Y, MIN_PAUSE_Y));
      
    int fadeTime = 0;
    short value = map(intensity, 0, 100, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    
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
      value -= map(intensity, INTENSITY_MIN, INTENSITY_MAX, MIN_FADE, MAX_FADE);
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
