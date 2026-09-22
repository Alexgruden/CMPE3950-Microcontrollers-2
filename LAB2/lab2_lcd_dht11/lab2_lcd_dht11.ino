/*
Alexander Gruden
University of Vermont
CMPE 3990A: Microcontrollers2
Lab 2
16 September 2026
The purpose of this program is to have a polling approach to read a temp and to detect if a button is pressed.
If there is a button pressed, the program will change the readings from C to F and print it on the LCD screen.
*/

#include <U8g2lib.h>
#include <DHT11.h>
#include <Wire.h>

constexpr uint8_t BUTTON = A0;
constexpr bool BUTTON_PRESSED = LOW;
constexpr bool BUTTON_RELEASED = HIGH;
constexpr uint8_t DHT_PIN = A1;

DHT11 dht11(DHT_PIN);

//Constructor handles Screen with i2c on A4 A5 pins
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// ----------STATES---------

// Information state
bool useFahrenheit = false; // false = celsius, true = fahrenheit
bool displayDirty = true; // Need to redraw screen? if yes then do so once at the start of the loop
char tempLabel = 'C'; // Temperature label printed on the lcd (C or F)

// Button state
bool lastButtonReading = HIGH; // pull-up: released = HIGH
unsigned long lastPressTime = 0; // When last press was
const unsigned long lockoutMs = 40; // Ignore futher presses for this duration (bounce guard)

// Sensor read timing
unsigned long lastReadTime = 0;
const unsigned long readInterval = 2000; // ms

// Last readings
int lastTempC = 0;
int lastHumidity = 0;
int displayTemp = 0;
bool validReading = false; //Flag if the reading was bad
int result = 0;

void setup() {
  Serial.begin(9600);
  u8g2.begin(); // Initialize LCD

  pinMode(BUTTON, INPUT_PULLUP);
  
  delay(1000);
}

void handleButton() {
  // Read difference from last pressed and determine if its earlier than lockout time
  // Trigger the change when button goes from high to low
  bool readButton = digitalRead(BUTTON);

  if ((millis() - lastPressTime) > lockoutMs) {
    if (readButton == BUTTON_PRESSED && lastButtonReading == BUTTON_RELEASED) {
      useFahrenheit = !useFahrenheit; // Switch to the opposite of what is current stored in variable.
      displayDirty = true; //Flag for update
      lastPressTime = millis(); // Update current time
    }
  }
  lastButtonReading = readButton; // Update the button with the current readings 
}

void readSensor() {
  // Read temp
  if ((millis() - lastReadTime) > readInterval) {
    lastReadTime = millis();

    result = dht11.readTemperatureHumidity(lastTempC, lastHumidity);

      //Sanitize the readings and flag accordingly
    if (result == 0) {
          validReading = true;
          displayDirty = true;
    } 
    else {
          validReading = false;
          displayDirty = true;
    }
  }
}
void updateDisplay() {
  // Check if the display needs updating, if not control flow skips all updatinug logic
  if (displayDirty) {
    // Temperature conversion logic
    if (useFahrenheit) {
      displayTemp = (lastTempC * 9 / 5) + 32;
      tempLabel = 'F';
    }
    else {
      displayTemp = lastTempC;
      tempLabel = 'C';
    }
    u8g2.clearBuffer();					// clear the internal memory

    if (validReading) {
      u8g2.setFont(u8g2_font_ncenB08_tr); // smaller font
      u8g2.setCursor(0, 10);
      u8g2.print("Temp & Humidity");

      u8g2.setFont(u8g2_font_logisoso24_tf); // larger font for readability
      u8g2.setCursor(0, 42); 
      u8g2.print(displayTemp);
      u8g2.setCursor(45, 42);
      u8g2.print(tempLabel);

      u8g2.setFont(u8g2_font_ncenB08_tr); // reset to smaller font 
      u8g2.setCursor(0, 60);
      u8g2.print("Humidity: ");
      u8g2.print(lastHumidity);
      u8g2.print("%");
    }
    else {
      // Print Error if temp reading is bad
      u8g2.setFont(u8g2_font_logisoso24_tf);
      u8g2.setCursor(0, 42);
      u8g2.print("ER");
      u8g2.print(result);
    }
    u8g2.sendBuffer(); // transfer internal memory to the display
    displayDirty = false;
  }
}

void loop() {
  handleButton();
  readSensor();
  updateDisplay();
}
