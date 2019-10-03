// fill array with randomly placed colors based on intensity
void mixLedArrayEven(int LEDfringe) {
  // turning off front fringe
  for (int i = 0; i < LEDfringe * LEDSTEP; i += 1) {
    ledMix[i] = 6;
  }
  // turning off end fringe
  for (int i = NUMPIXELS_DOTSTAR1 - LEDfringe * LEDSTEP; i < NUMPIXELS_DOTSTAR1; i += 1) {
    ledMix[i] = 6;
  }
  // setting all others to green
  for (int i = LEDfringe; i < NUMPIXELS_DOTSTAR1 - LEDfringe; i += 1) {
    ledMix[i] = 0;
  }

  // Calculate number of total LEDs in use
  float totalLEDs = (NUMPIXELS_DOTSTAR1 - LEDfringe * 2) / LEDSTEP;
  // How many LEDs to color
  byte coloredLEDs = map(intensity, 0, INTENSITY_MAX, 0, totalLEDs);
  Serial.println(String("mixing led count:") + coloredLEDs);
  byte j = 0;
  //essential: this is where colored leds get mixed in
  // randomly color until coloredLEDs is reached
  while (j < coloredLEDs) {
    int ledI = random(LEDfringe, NUMPIXELS_DOTSTAR1 - LEDfringe) / LEDSTEP;
    if (ledMix[ledI * LEDSTEP] == 0) {
      ledMix[ledI * LEDSTEP] = 1;
      j++;
    }
  }
  // output to serial
  for (int i = 0; i < NUMPIXELS_DOTSTAR1; i += LEDSTEP) {
    if (ledMix[i] == 6) Serial.print(6);
    if (ledMix[i] == 1) Serial.print(1);
    else Serial.print(0);
  }
  Serial.println();
}


void displayTemperature(bool hotOrCold) {
  for (int i = 0; i < NUMPIXELS_DOTSTAR1; i += LEDSTEP) {
    if (ledMix[i] == 1) {
#ifdef DEBUG
      Serial.print(1);
#endif
      if (hotOrCold == 1) stripD1.setPixelColor(i, stripD1.Color(255, 0, 0)); // set red pixel
      else stripD1.setPixelColor(i, stripD1.Color(0, 255, 0)); // set blue pixel
    }
    else if (ledMix[i] == 0) {
#ifdef DEBUG
      Serial.print(0);
#endif
      stripD1.setPixelColor(i, stripD1.Color(0, GREEN_BRIGHT, 0)); // set green pixel
    }
    else if (ledMix[i] == 6) {
#ifdef DEBUG
      Serial.print(6);
#endif
      stripD1.setPixelColor(i, stripD1.Color(0, 0, 0)); // turn pixel off
    }
  }
  stripD1.show();
}

void turnOff() {
  stripD1.fill(stripD1.Color(0, 0, 0));
  stripD1.show();
  //Serial.println("*lights off");
}

//void mixLedArray() {
//  // fill array with randomly placed colors based on intensity and chance
//  Serial.println("mixing leds:");
//  for (int i = 0; i < NUMPIXELS_DOTSTAR1; i += LEDSTEP) {
//    if (random(70) < intensity) ledMix[i] = 1;
//    else ledMix[i] = 0;
//    Serial.print(ledMix[i]);
//  }
//  Serial.println();
//}


//void fillWhite(byte intensity) {
//  stripD1.fill(stripD1.Color(0, 0, 0, stripD1.gamma8(intensity)));
//  stripD1.show();
//  Serial.print("*white on: ");
//  Serial.println(intensity);
//}
