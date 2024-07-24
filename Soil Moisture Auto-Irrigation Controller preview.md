# Soil Moisture Control Code

This Arduino project monitors soil moisture levels and uploads the data to ThingSpeak for remote tracking. It includes a relay module to control an irrigation system, activating it when soil moisture drops below a specified threshold. Ideal for automated irrigation in agricultural applications and smart gardening.

## Include Libraries
```cpp
#include <Arduino.h>
#include <Wire.h>
#include <ThingSpeak.h>
#include <WiFi.h> // Include the WiFi library
```
## Define Pin Constants
```cpp
#define RELAY_PIN D12  // Define the relay pin as D12
#define SOIL_SENSOR_PIN A0  // Analog pin connected to the sensor
```
## ThingSpeak Configuration
```cpp
unsigned long myChannelNumber = 2591283;
const char *myWriteAPIKey = "84QHA0LT3CZ25OK3";
```
## WiFi Configuration
```cpp
const char *ssid = "agrotech"; // Your WiFi SSID name
const char *password = "1Afuna2gezer"; // WiFi password
WiFiClient client;
```
## Calibration and Threshold Settings
```cpp
int dryValue = 0;  // Calibrate this value
int wetValue = 1023;  // Calibrate this value
int threshold = 40;  // Threshold moisture percentage to turn the relay on
```
## Setup Function
```cpp
void setup() {
  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);

  // Initialize serial communication for debugging
  Serial.begin(115200);

  // Initialize WiFi
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

  // Soil moisture sensor calibration
  Serial.println("Calibrating soil moisture sensor...");
  
  // Measure dry value
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
```
## Loop Function

```cpp
void loop() {
  // Read the current soil moisture value from the sensor
  int sensorValue = analogRead(SOIL_SENSOR_PIN);
  
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

  // Delay for 5 minutes (300,000 milliseconds)
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

  // Wait for a second before taking another reading
  delay(1000);
}
```

[Click to get back to the first page](https://github.com/NetaCohenSimhi/composensor/blob/main/README.md)
