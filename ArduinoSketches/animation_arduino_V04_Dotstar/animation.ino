void mixLedArray() {
  // fill array with randomly placed colors based on intensity and chance
  Serial.println("mixing leds:");
  for (int i = 0; i < NUMPIXELS_DOTSTAR1; i += LEDSTEP) {
    if (random(70) < intensity) ledMix[i] = 1;
    else ledMix[i] = 0;
    Serial.print(ledMix[i]);
  }
  Serial.println();
}

// fill array with randomly placed colors based on intensity
void mixLedArrayEven() {
  // setting all to white first
  for (int i = 0; i < NUMPIXELS_DOTSTAR1; i += LEDSTEP) {
    ledMix[i] = 0;
  }
  // how many leds to color
  byte ledCount = map(intensity, 0, INTENSITY_MAX, 0, NUMPIXELS_DOTSTAR1 / LEDSTEP);
  Serial.println(String("mixing led count:") + ledCount);
  byte j = 0;
  // randomly color until ledCount is reached
  while (j < ledCount) {
    byte ledI = random(NUMPIXELS_DOTSTAR1 / LEDSTEP) * LEDSTEP;
    if (ledMix[ledI] == 0) {
      ledMix[ledI] = 1;
      j++;
    }
  }
  // output to serial
  for (int i = 0; i < NUMPIXELS_DOTSTAR1; i += LEDSTEP) {
    if (ledMix[i] == 1) Serial.print(1);
    else Serial.print(0);
  }
  Serial.println();
}


void mixInHeat(byte LEDnum, byte LEDfringe, bool* ledMix) {
  // output to serial
  for (int i = 0; i < LEDnum; i += LEDSTEP) {
    if (ledMix[i] == 1) {
//      Serial.print(1);
      stripD1.setPixelColor(i, stripD1.Color(255, 0, 0)); // set red pixels
    }
    else {
//      Serial.print(0);
      stripD1.setPixelColor(i, stripD1.Color(0, GREEN_BRIGHT, 0));
    }
  }
  stripD1.show();
}

void mixInCold(byte LEDnum, byte LEDfringe, bool* ledMix) {
  for (int i = 0; i < LEDnum; i += LEDSTEP) {
    if (ledMix[i] == 1) {
      stripD1.setPixelColor(i, stripD1.Color(0, 0, 255)); // set blue pixels
#ifdef DEBUG
      Serial.print("1");
#endif
    }
    else {
      stripD1.setPixelColor(i, stripD1.Color(0, GREEN_BRIGHT, 0));
#ifdef DEBUG
      Serial.print("0");
#endif
    }
  }
#ifdef DEBUG
  Serial.println();
#endif
  stripD1.show();
}

//void fillWhite(byte intensity) {
//  stripD1.fill(stripD1.Color(0, 0, 0, stripD1.gamma8(intensity)));
//  stripD1.show();
//  Serial.print("*white on: ");
//  Serial.println(intensity);
//}

void turnOff() {
  stripD1.fill(stripD1.Color(0, 0, 0));
  stripD1.show();
  //Serial.println("*lights off");
}
