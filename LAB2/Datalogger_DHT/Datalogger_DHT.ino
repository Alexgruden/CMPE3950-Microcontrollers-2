/*
Alexander Gruden
University of Vermont
CMPE 3990A: Microcontrollers2
Lab 2
16 September 2026
The purpose of this program is to log data from a DHT11 sensor
*/


#include <SPI.h>
#include <SD.h>
#include <DHT11.h>

DHT11 dht11(8);

const int chipSelect = 10;


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
  while (!Serial);

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true);
  }

  Serial.println("initialization done.");
}

void loop() {
  int temp = 0;
  int humidity = 0;

  // read three sensors and append to the string:
  int result = dht11.readTemperatureHumidity(temp, humidity);
  
  String dataString = String(temp) + "," + String(humidity);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(500);
}
