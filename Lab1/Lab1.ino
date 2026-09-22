const uint8_t LATCH = 0; // Storage register clock input/STCP
const uint8_t OE = 1;    // OE
const uint8_t DATA = 2;  // serial input/DS
const uint8_t CLOCK = 3; // SHCP
const uint8_t POT = A2;  // Potentiometer

void setup() {
  // Set pins to output
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(OE, OUTPUT);

  // Active LOW for Output Enable (OE)
  digitalWrite(OE, LOW);
}

void loop() {
  for (int i = 0; i < 8; i++) {
    // 1 for first iteration, 0 otherwise
    if (i == 0) {
      digitalWrite(DATA, HIGH);
    } else {
      digitalWrite(DATA, LOW);
    }

    // Shift signal
    digitalWrite(CLOCK, HIGH);
    digitalWrite(CLOCK, LOW);

    // Copies to storage register
    digitalWrite(LATCH, HIGH);
    digitalWrite(LATCH, LOW);

    delay(500);
  }
  delay(1000);
}




// void loop() {
//   // Read Potentiometer
//   int potLevel = (analogRead(POT) / 4);

//   // Pattern to be placed on the LEDs
//   // uint8_t pattern = (uint8_t) ((1 << potLevel) - 1);

//   digitalWrite(LATCH, LOW);
//   shiftOut(DATA, CLOCK, MSBFIRST, potLevel); // Write to shift register
//   digitalWrite(LATCH, HIGH);

//   delay(30);
// }




