#include <ESP8266WiFi.h>

const char* ssid = "BroadBand_D-Link-615";
const char* password = "riki@123";

// Pin definitions
const int soilMoisturePin = A0; // Analog pin for soil moisture sensor
const int pumpPin = D1;         // Digital pin for controlling water pump

// Threshold for soil moisture (adjust as needed)
const int moistureThreshold = 500;

int pump_auto = 0;

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
  // int moisturePercentage = map(moistureLevel, 1024, 556, 0, 100);
  int moisturePercentage = map(moistureLevel, 1024, 556, 0, 100);

  // Send moisture data over serial connection
  Serial.println(moisturePercentage);

  // Check if soil is dry
  if (pump_auto == 0) {
    if (moisturePercentage < 40) {
      handleRelayOn();
    } else {
      handleRelayOff();
    }
  }

    if (Serial.available() > 0) {
    char receivedByte = Serial.read();

      // Map received byte to function
      switch (receivedByte) {
        case '1':
          handleRelayOn();
          pump_auto = 1;
          break;
        case '0':
          handleRelayOff();
          pump_auto = 1;
          break;
        case '2':
          pump_auto = 0;
          break;
        default:
          // Handle unknown command
          Serial.println("Unknown command");
          break;
      }
    }

  delay(800); // Wait for 1 minute before taking the next reading
}