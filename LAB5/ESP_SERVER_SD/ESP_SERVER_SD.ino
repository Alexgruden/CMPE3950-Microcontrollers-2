/**
Code Modified By: Alexander Gruden
CMPE 3990: Microcontrollers 2
11/02/2026
*/

// Original Documentation by author:
/* Dr. Tony Barsic
 * University of Vermont
 * Department of Electrical Engineering
 * CMPE 3990: Microcontrollers 2
 * 22 Oct 2026
 *
 * BASIC WIFI COMMUNICATION between two ESP32's.
 * The client sends a text message once per second;
 * the server prints it to the serial monitor.
 *
 * ESP32 #1 is the Access Point (AP).
 *    It creates the network and gets the default IP (192.168.4.1).
 * ESP32 #2 is a station.
 *    That means it joins the network created by ESP32 #1.
 *
 * ESP32 #1 is also the server. The server is the one that waits and listens.
 * ESP32 #2 is the client. It initiates the conversation.
 *
 * This is the code for ESP32 #1 (AP + server).
 * The circuit:
 * An ESP32, 3rd party module and dev kit. Chip: ESP32-D0WD-V3
 * Connect by USB so you can watch the serial monitor. That's it!
 *
 * The client sends its text as a URL parameter:
 *     http://192.168.4.1/msg?text=hello%20world.%2012345%20ms.
 * (%20 is a URL-encoded space; the server decodes it automatically.)
 */

// INITIALIZATION ===+=== ===+=== ===+=== ===+=== ===+=== ===+=== ===+===
// Import SD card libraries
#include <SPI.h>
#include <SD.h>
// Declare File object 
File myFile;

// Import wifi libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
// Set access point network credentials
const char* ssid = "ESP32-Access-Point-Agruden";
const char* password = "123456789";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Chip Select Pin
const int CS_PIN = 5;

// SETUP ===+=== ===+=== ===+=== ===+=== ===+=== ===+=== ===+=== ===+===
void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();Serial.println("===+=== ===+=== ===+===");Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  Serial.print("Initializing SD card...");

  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");


  // set up the server response to client request
  server.on("/msg", HTTP_GET, [](AsyncWebServerRequest *request){
    // do the work first: get the text and print it...
    if (request->hasParam("text")) {
      // Log to serial for debugging
      String text = request->getParam("text")->value();
      Serial.print("Received from client: ");
      Serial.println(text);

      // Open sd card in append to add to current log or return error message
      File dataFile = SD.open("/log.txt", FILE_APPEND);
      if (dataFile) {
        dataFile.println(text);
        dataFile.close();
        request->send(200, "text/plain", "LOGGED SUCCESSFULLY\n"); 
      } else {
        Serial.println("Could not open /templog.txt on SD card");
        request->send(400, "text/plain", "SD LOG ERROR\n");
      }
    } else {
      Serial.println("Request with no text parameter");
      request->send(400, "text/plain", "missing text\n");
    }
  });

  // Start server
  server.begin();
}

// LOOP ===+=== ===+=== ===+=== ===+=== ===+=== ===+=== ===+=== ===+===
void loop() {
  // Nothing to do here: the server runs in the background
  // and calls the function above whenever a request arrives.
}

// ===+===  END OF SKETCH  ===+=== ===+=== ===+=== ===+=== ===+===
// ===+=== ===+=== ===+===  END OF SKETCH  ===+=== ===+=== ===+===
// ===+=== ===+=== ===+=== ===+=== ===+===  END OF SKETCH  ===+===
