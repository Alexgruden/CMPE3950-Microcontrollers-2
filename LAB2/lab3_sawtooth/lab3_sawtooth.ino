/*
Alexander Gruden
University of Vermont
CMPE 3990A: Microcontrollers2
Lab 3
24 September 2026
This is a simple program which creates a sawtooth pattern on an oscilloscope.
*/

#include <Adafruit_DotStar.h>

// turn off the on-board the DOTSTAR
Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

int sineValue = 0;

void setup() {
  // turn off the DOTSTAR
  strip.begin(); // Initialize the DotStar
  strip.setBrightness(80); // 0-255; keep it low, this LED is bright up close
  strip.setPixelColor(0, 0, 0, 0); // (PixNum,R,G,B) OFF (brightness irrelevant)
  strip.show(); // send the data!

  analogWriteResolution(10);
}

void loop() {
  // 1 Loop = 1 Period
  for (int n = 0; n < 1024; n++) {
    analogWrite(A0, n);
  }
}
