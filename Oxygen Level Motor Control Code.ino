/*
Class: Agrotech Lab
Date: 23-07-2024
This code controls a motor based on oxygen levels and sends the data to ThingSpeak.
It connects to a WiFi network, reads (simulated) oxygen levels, controls the motor accordingly,
and uploads the data to the ThingSpeak platform for monitoring.
*/

#include <Arduino.h>
#include <Wire.h>
#include <ThingSpeak.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h> // Include the WiFi library

unsigned long myChannelNumber = 2591283; // ThingSpeak channel number
const char *myWriteAPIKey = "84QHA0LT3CZ25OK3"; // ThingSpeak write API key
const char *ssid = "agrotech"; // WiFi SSID name
const char *password = "1Afuna2gezer"; // WiFi password

// Motor control pins
const int RPWM = D9;  // Right PWM pin
const int LPWM = D10; // Left PWM pin
const int REN = D11;  // Right enable pin
const int LEN = D11;  // Left enable pin

// Define PWM channels
const int pwmChannelRPWM = 0; // PWM channel for right PWM
const int pwmChannelLPWM = 1; // PWM channel for left PWM

WiFiClient client;

// Generates a random number between 0 and 21 inclusive to simulate oxygen level
float generateRandomOxygenLevel() {
  return random(0, 21); 
}

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  
  // Disconnect and connect WiFi
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  ThingSpeak.begin(client); // Initialize ThingSpeak
  
  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
  
  // Initialize motor control pins as outputs
  pinMode(REN, OUTPUT);
  pinMode(LEN, OUTPUT);
  
  // Enable the motor driver
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);

  // Configure PWM functionalities
  ledcSetup(pwmChannelRPWM, 1000, 8); // 1 kHz PWM, 8-bit resolution for right PWM
  ledcSetup(pwmChannelLPWM, 1000, 8); // 1 kHz PWM, 8-bit resolution for left PWM
  
  // Attach the channels to the GPIOs to be controlled
  ledcAttachPin(RPWM, pwmChannelRPWM);
  ledcAttachPin(LPWM, pwmChannelLPWM);
  
  Serial.println("Motor driver initialized");
}

void loop() { 
  // Generate random oxygen level (for testing)
  float oxygenLevel = generateRandomOxygenLevel();
  Serial.print("Oxygen Level (%): ");   
  Serial.println(oxygenLevel);    

  // Set the values to be sent to ThingSpeak
  ThingSpeak.setField(3, oxygenLevel);

  // Send the data to ThingSpeak
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // Check oxygen level condition to control the motor
  if (oxygenLevel < 5.0) {
    // Run motor in one direction
    ledcWrite(pwmChannelRPWM, 255); // Full speed on right PWM
    ledcWrite(pwmChannelLPWM, 0);   // Stop left PWM
    Serial.println("Motor running due to low oxygen level.");
    delay(2000); // Run motor for 2 seconds
    
    // Run motor in the opposite direction
    ledcWrite(pwmChannelRPWM, 0);
    ledcWrite(pwmChannelLPWM, 255); // Full speed on left PWM
    delay(3000); // Run motor in opposite direction for 3 seconds
    
    // Stop the motor
    ledcWrite(pwmChannelRPWM, 0);
    ledcWrite(pwmChannelLPWM, 0);
  } else {
    // Stop the motor
    ledcWrite(pwmChannelRPWM, 0);
    ledcWrite(pwmChannelLPWM, 0);
    Serial.println("Motor stopped due to sufficient oxygen level.");
  }

  // Delay before uploading next set of data to ThingSpeak
  // 300000 milliseconds = 5 minutes (30,000 milliseconds = 30 seconds for frequent updates)
  delay(30000);
}
