#include <ESP8266WiFi.h>

const char* ssid = "BroadBand_D-Link-615";
const char* password = "riki@123";

// Pin definitions
const int soilMoisturePin = A0; // Analog pin for soil moisture sensor
const int pumpPin = D1;         // Digital pin for controlling water pump

// Threshold for soil moisture (adjust as needed)
const int moistureThreshold = 500;

void handleRelayOff() {
  digitalWrite(pumpPin, LOW);
}

void handleRelayOn() {
  digitalWrite(pumpPin, HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {

  // Read soil moisture level
  int moistureLevel = analogRead(soilMoisturePin);
  int moisturePercentage = map(moistureLevel, 1024, 556, 0, 100);

  // Send moisture data over serial connection
  Serial.println(moisturePercentage);

  // Check if soil is dry
  if (moisturePercentage < 60) {

    // Activate water pump
    digitalWrite(pumpPin, HIGH);
    delay(1000); // Adjust watering duration as needed (5 seconds here)

    // Deactivate water pump
    digitalWrite(pumpPin, LOW);
  }

    if (Serial.available() > 0) {
    char receivedByte = Serial.read();

      // Map received byte to function
      switch (receivedByte) {
        case '1':
          handleRelayOn();
          break;
        case '0':
          handleRelayOff();
          break;
        default:
          // Handle unknown command
          Serial.println("Unknown command");
          break;
      }
    }

  delay(500); // Wait for 1 minute before taking the next reading
}