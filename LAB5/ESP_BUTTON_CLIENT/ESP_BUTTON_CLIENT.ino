/**
Code Modified By: Alexander Gruden
CMPE 3990: Microcontrollers 2
11/02/2026
*/


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
 * This is the code for ESP32 #2 (station + client).
 * The circuit:
 * An ESP32, 3rd party module and dev kit. Chip: ESP32-D0WD-V3
 * It's designed to run without USB connection:
 *     - breadboard power supply, 5V, to VIN
 *     - both GND to GND (redundant)
 *   That's it! (USB works too, if you want to watch the serial monitor.)
 *
 * Onboard LED (GPIO2) status codes:
 *   steady fast blinking  = connecting to WiFi
 *   1 short blink         = message sent, server replied
 *   5 fast blinks         = send failed / WiFi disconnected
 *
 * Once per second, it sends "hello world. xxxxx ms." where xxxxx is
 * millis(). The text goes in the URL, so spaces are sent as %20:
 *     http://192.168.4.1/msg?text=hello%20world.%2012345%20ms.
 */

// INITIALIZATION ===+=== ===+=== ===+=== ===+=== ===+=== ===+=== ===+===
// Import wifi libraries
#include <WiFi.h>
#include <HTTPClient.h>
// Set access point network credentials
const char* ssid = "ESP32-Access-Point-Agruden";
const char* password = "123456789";

// timing for sending messages
const unsigned long SEND_INTERVAL_MS = 1000;  // one message per second
unsigned long lastSend = 0;

// We're going to use the on-board (OB) LED as the status indicator
const byte PIN_LED_OB = 2;
bool LED_OB_status = 1;

// Button configuration
const int BUTTON_PIN = 32;
bool lastButtonState = HIGH;

// FUNCTION FOR LED BLINKING ===+=== ===+=== ===+=== ===+=== ===+=== ===+===
void blinkLED(int pinLED, int nBlinks, int halfPeriodMs) {
  for(int k=0; k<nBlinks; k++){
    digitalWrite(pinLED, HIGH); delay(halfPeriodMs);
    digitalWrite(pinLED, LOW); delay(halfPeriodMs);
  }
}

// FUNCTION FOR HTTP-GET ===+=== ===+=== ===+=== ===+=== ===+=== ===+===
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // The Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP GET request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

// SETUP ===+=== ===+=== ===+=== ===+=== ===+=== ===+=== ===+=== ===+===
void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();Serial.println("===+=== ===+=== ===+===");Serial.println();

  // initialize the (on-board) LED pin
  pinMode(PIN_LED_OB, OUTPUT);
  digitalWrite(PIN_LED_OB,LED_OB_status);
  // Add new button pin init with pullup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // connect to the other ESP32's WiFi network
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(100);
    Serial.print(".");
    // blink OB-LED
    LED_OB_status = !LED_OB_status;
    digitalWrite(PIN_LED_OB,LED_OB_status);
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  // turn off OB-LED; from here on it only blinks for sends
  LED_OB_status = 0;
  digitalWrite(PIN_LED_OB,LED_OB_status);
}

// LOOP ===+=== ===+=== ===+=== ===+=== ===+=== ===+=== ===+=== ===+===
void loop() {
  // Read the current state of the button
  bool currentButtonState = digitalRead(BUTTON_PIN);

  // Do the first check to see if there is a button press
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50); // Delay to check debounce

    //Check again to confirm reading
    if (digitalRead(BUTTON_PIN) == LOW) {
    // If the button reading was confirmed move to the following:
    // Check WiFi connection status
      if(WiFi.status()== WL_CONNECTED ){ 
        // If wifi is connected, send request
        String reply = httpGETRequest("http://192.168.4.1/msg");
        Serial.print("Server reply: ");
        Serial.println(reply);
        // httpGETRequest returns "--" if the request failed
        if (reply == "--") blinkLED(PIN_LED_OB, 5, 30);   // 5 fast blinks = failed
        else               blinkLED(PIN_LED_OB, 1, 50);   // 1 short blink = sent
      }
      else {
        Serial.println("WiFi Disconnected");
        blinkLED(PIN_LED_OB, 5, 30);   // 5 fast blinks = no WiFi
      }
    }
  }
  // Update button state at the end of every loop
  lastButtonState = currentButtonState;  
}

// ===+===  END OF SKETCH  ===+=== ===+=== ===+=== ===+=== ===+===
// ===+=== ===+=== ===+===  END OF SKETCH  ===+=== ===+=== ===+===
// ===+=== ===+=== ===+=== ===+=== ===+===  END OF SKETCH  ===+===
