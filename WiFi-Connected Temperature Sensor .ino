/*
Class: Agrotech Lab
Date: 23-07-2024
This code reads temperature data from a DS18B20 sensor, connects to WiFi, and sends the
temperature data to ThingSpeak for remote monitoring. The data is uploaded at regular intervals.
*/

#include <Arduino.h>
#include <Wire.h>
#include <ThingSpeak.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h> // Include the WiFi library

// ThingSpeak channel configuration
unsigned long myChannelNumber = xxxxxx;
const char *myWriteAPIKey = "xxxxxxxxxxx";
const char *ssid = "xxxxxxxxxxxx"; // WiFi SSID name
const char *password = "xxxxxxxx"; // WiFi password

// Specify the GPIO pin where the DS18B20 sensor is connected
const int oneWireBus = A4;     

// Create a OneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass the OneWire reference to the Dallas Temperature sensor instance
DallasTemperature sensors(&oneWire);

WiFiClient client;

void setup() {
  Serial.begin(115200);  // Initialize serial communication at 115200 baud
  sensors.begin();  // Initialize communication with the DS18B20 sensor
  
  // Connect to WiFi
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  ThingSpeak.begin(client); // Initialize ThingSpeak
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to WiFi...");
  Serial.println(ssid);
  Serial.println();
}

void loop() {
  // Request the latest temperature readings from the DS18B20 sensor
  sensors.requestTemperatures(); 
  
  // Get the temperature readings in Celsius for the first sensor (index 0)
  float temperature = sensors.getTempCByIndex(0);
  
  // Print the temperature in Celsius to the Serial Monitor
  Serial.print(temperature);

  if (!isnan(temperature)) {  // Check if temperature is not NaN (not a number)
    Serial.print("temp_deg_C:");  // Print temperature label
    Serial.print(temperature);    // Print temperature value
    Serial.print("\t");           // Write a tab space
  } else {
    // Print an error message if temperature reading fails
    Serial.println("Failed to read temperature");
  }
  
  // Set the values to be sent to ThingSpeak
  ThingSpeak.setField(1, temperature);

  // Send the data to ThingSpeak
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // Print a message to the serial monitor indicating that the data has been uploaded
  Serial.println("Uploaded to ThingSpeak server.");

  // Wait for the specified amount of time before uploading the next set of data
  // 300000 milliseconds = 5 minutes
  delay(300000);
}
