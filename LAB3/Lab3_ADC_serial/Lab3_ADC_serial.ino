/*
Alexander Gruden
University of Vermont
CMPE 3990A: Microcontrollers2
Lab 2
24 September 2026
The purpose of this program is to generate data in the serial monitor to be based to a software to graph it.
The noise will be viewed in a line graph form then placed into a histogram to see the buckets of the signals voltage.
*/

#include <Adafruit_DotStar.h>

// turn off the on-board the DOTSTAR
Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

const int PIN_ADC = A1;
const int ADC_BITS = 12;
const long BAUD_RATE = 115200;
const int DATA_POINTS = 4000;


void setup() {
  // turn off the DOTSTAR
  strip.begin(); // Initialize the DotStar
  strip.setBrightness(80); // 0-255; keep it low, this LED is bright up close
  strip.setPixelColor(0, 0, 0, 0); // (PixNum,R,G,B) OFF (brightness irrelevant)
  strip.show(); // send the data!

  Serial.begin(BAUD_RATE);
  analogReadResolution(ADC_BITS);
}

void loop() {
  Serial.println("Starting Recording");
  for (int i = 0; i < DATA_POINTS; i++) {
    // Format output for visualization purposes
    Serial.print(i);
    Serial.print(",");
    Serial.println(analogRead(PIN_ADC));
  }
  Serial.println("Recording Ended");
  delay(10000);
}
