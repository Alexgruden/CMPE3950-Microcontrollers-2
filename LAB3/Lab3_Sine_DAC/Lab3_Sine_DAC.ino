/*
Alexander Gruden
University of Vermont
CMPE 3990A: Microcontrollers2
Lab 3
24 September 2026
The purpose of this program is to create a sine wave on an oscilloscope. 
Its done by computing the voltage to be output live in each iteration of the loop
*/

#include <Adafruit_DotStar.h>

// turn off the on-board the DOTSTAR
Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

int sineValue = 0;
const int TRIGGER_PIN = 3;

void setup() {
  // turn off the DOTSTAR
  strip.begin(); // Initialize the DotStar
  strip.setBrightness(80); // 0-255; keep it low, this LED is bright up close
  strip.setPixelColor(0, 0, 0, 0); // (PixNum,R,G,B) OFF (brightness irrelevant)
  strip.show(); // send the data!

  pinMode(TRIGGER_PIN, OUTPUT);

  analogWriteResolution(10);
}

void loop() {
  // 1 Loop = 1 Period
  digitalWrite(TRIGGER_PIN, HIGH);
  for (int n = 0; n < 1024; n++) {
    float rad = (float)n * ((2.0 * 3.1415f) / 1024.0); // Compute rad based on loop iteration
    sineValue = (int)(511.5 + 511.5 * sin(rad)); // Calculate the sin value to be output
    //digitalWrite(A4, HIGH);
    analogWrite(A0, sineValue);
    //digitalWrite(A4, LOW);
  }
  digitalWrite(TRIGGER_PIN, LOW);
  delay(100);
}