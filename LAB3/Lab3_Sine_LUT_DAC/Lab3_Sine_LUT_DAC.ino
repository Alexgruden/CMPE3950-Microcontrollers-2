/*
Alexander Gruden
University of Vermont
CMPE 3990A: Microcontrollers2
Lab 3
24 September 2026
The Purpose of this program is to expand on a program which made a sine wave on an oscilloscope.
It add functionality but precomputing a look up table which can be accessed during loop()
*/

#include <Adafruit_DotStar.h>

// turn off the on-board the DOTSTAR
Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

int sineValue = 0;
constexpr int LUT_SIZE = 1024; // Size of look up table
int sin_LUT[LUT_SIZE]; // Init array of size LUT_SIZE
constexpr uint8_t TIME_MARKER = 3;// Time Marker PIN


void setup() {
  // turn off the DOTSTAR
  strip.begin(); // Initialize the DotStar
  strip.setBrightness(80); // 0-255; keep it low, this LED is bright up close
  strip.setPixelColor(0, 0, 0, 0); // (PixNum,R,G,B) OFF (brightness irrelevant)
  strip.show(); // send the data!

  pinMode(TIME_MARKER, OUTPUT);
  
  analogWriteResolution(10);

  // Pre-computed LUT
  for (int i = 0; i < LUT_SIZE; i++) {
    float rad = (float)i * ((2.0 * 3.1415f) / LUT_SIZE); // Calculate rad 
    sin_LUT[i] = (int)(511.5 + 511.5 * sin(rad)); // Assign the sin value at index i
  }
}

void loop() {
  // 1 Loop = 1 Period
  digitalWrite(TIME_MARKER, HIGH);
  for (int n = 0; n < LUT_SIZE; n++) {
    analogWrite(A0, sin_LUT[n]); // Write the value from the precomputed LUT
  }
  digitalWrite(TIME_MARKER, LOW);
}