/*
Class: Agrotech Lab
Date: 24-07-2024
This code monitors soil moisture levels and uploads the data
to ThingSpeak for remote tracking.
It includes a relay module to control an irrigation system,
activating it when soil moisture drops below a specified threshold.
Ideal for automated irrigation in agricultural applications and smart gardening
*/

#include <Arduino.h>
#include <Wire.h>
#include <ThingSpeak.h>
#include <WiFi.h> // Include the WiFi library

#define RELAY_PIN D12  // Define the relay pin as D12
#define SOIL_SENSOR_PIN A0  // Analog pin connected to the sensor

unsigned long myChannelNumber = 2591283;
const char *myWriteAPIKey = "84QHA0LT3CZ25OK3";
const char *ssid = "agrotech"; // Your WiFi SSID name
const char *password = "1Afuna2gezer"; // WiFi password
int dryValue = 0;  // Calibrate this value
int wetValue = 1023;  // Calibrate this value
int threshold = 40;  // Threshold moisture percentage to turn the relay on

WiFiClient client;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(115200);
  // Initialize communication with the DS18B20 sensor
  sensors.begin();
  
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  ThingSpeak.begin(client);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
  Serial.println("Calibrating soil moisture sensor...");
  
  //Measure dry value
  Serial.println("Insert the sensor into dry soil and press 'd'");
  while (Serial.read() != 'd') {
    delay(100);
  }
  dryValue = analogRead(SOIL_SENSOR_PIN);
  Serial.print("Dry value set to: ");
  Serial.println(dryValue);
  
  // Measure wet value
  Serial.println("Insert the sensor into wet soil and press 'w'");
  while (Serial.read() != 'w') {
    delay(100);
  }
  wetValue = analogRead(SOIL_SENSOR_PIN);
  Serial.print("Wet value set to: ");
  Serial.println(wetValue);
 
  // Give some time before starting the loop
  delay(2000);
}

void loop() {
  int sensorValue = analogRead(SOIL_SENSOR_PIN); // Read the current soil moisture value from the sensor.
  
  // Map the sensor value to a percentage
 int moisturePercent = map(sensorValue, dryValue, wetValue, 0, 100);
  
  // Ensure the percentage is within 0-100%
  moisturePercent = constrain(moisturePercent, 0, 100);
  
  // Print the moisture percentage
  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");
  
  // Set the values to be sent to ThingSpeak
  ThingSpeak.setField(2, moisturePercent);

  // Send the data to ThingSpeak
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // Print a message to the serial monitor indicating that the data has been uploaded
  Serial.println("Uploaded to ThingSpeak server.");

  delay(300000);

  // Control the relay based on soil moisture
  if (moisturePercent < threshold) {
    digitalWrite(RELAY_PIN, LOW);  // Turn the relay on (open)
    Serial.println("Relay ON");
    delay(2000);
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Turn the relay off (close)
    Serial.println("Relay OFF");
    delay(2000);
  }

  delay(1000);  // Wait for a second before taking another reading
}
