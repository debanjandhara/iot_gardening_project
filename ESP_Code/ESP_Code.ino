#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "BroadBand_D-Link-615";
const char* password = "riki@123";

// Pin definitions
const int soilMoisturePin = A0; // Analog pin for soil moisture sensor
const int pumpPin = D1;         // Digital pin for controlling water pump

// Threshold for soil moisture (adjust as needed)
const int moistureThreshold = 500;

ESP8266WebServer server(80);

void handleRoot() {
  int moistureLevel = analogRead(soilMoisturePin);
  int moisturePercentage = map(moistureLevel, 1024, 556, 0, 100);

  String response = String(moisturePercentage);
  server.send(200, "text/plain", response);

  
}

void handleRelayOff() {
  digitalWrite(pumpPin, LOW);
  server.send(200, "text/plain", "Pump turned off");
}

void handleRelayOn() {
  digitalWrite(pumpPin, HIGH);
  server.send(200, "text/plain", "Pump turned on");
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

  // Setup web server
  // server.on("/", handleRoot);
  // server.on("/relay-on", handleRelayOn);
  // server.on("/relay-off", handleRelayOff);
  // server.begin();
  // Serial.println("HTTP server started");

  // Setup web server
  // server.on("/", HTTP_GET, handleRoot);
  

  // Setup web server
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    handleRoot();
  });

  server.on("/relay-on", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    handleRelayOn();
  });

  server.on("/relay-off", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    handleRelayOff();
  });


  server.begin();
  Serial.println("HTTP server started");


}

void loop() {
  server.handleClient();

  // Read soil moisture level
  int moistureLevel = analogRead(soilMoisturePin);
  int moisturePercentage = map(moistureLevel, 1024, 556, 0, 100);

  Serial.print("Moisture Level: ");
  Serial.println(moisturePercentage);

  // Check if soil is dry
  if (moisturePercentage < 60) {
    Serial.println("Soil is dry. Watering...");

    // Activate water pump
    digitalWrite(pumpPin, HIGH);
    delay(1000); // Adjust watering duration as needed (5 seconds here)

    // Deactivate water pump
    digitalWrite(pumpPin, LOW);
    Serial.println("Watering complete.");
  } else {
    Serial.println("Soil moisture is sufficient. No watering needed.");
  }

  delay(1000); // Wait for 1 minute before taking the next reading
}