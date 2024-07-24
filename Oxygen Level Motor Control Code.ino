/*
Class: Agrotech Lab
Date: 24-07-2024
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

unsigned long myChannelNumber = xxxxxx;
const char *myWriteAPIKey = "xxxxxxxxxxx";
const char *ssid = "xxxxxxxxxxxx"; // WiFi SSID name
const char *password = "xxxxxxxx"; // WiFi password


// Motor control pins
const int RPWM = D9;  // Right PWM
const int LPWM = D10;  // Left PWM
const int REN = D11;  // Right enable
const int LEN = D11;  // Left enable

// Define PWM channels
const int pwmChannelRPWM = 0;
const int pwmChannelLPWM = 1;

WiFiClient client;

// Generates a random number between 0 and 20 inclusive
float generateRandomOxygenLevel() {
  return random(0, 20); 
}

void setup() {
  Serial.begin(115200);
  
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
  
  // Initialize motor control pins as outputs
  pinMode(REN, OUTPUT);
  pinMode(LEN, OUTPUT);
  
  // Enable the motor driver
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);

  // Configure PWM functionalities
  ledcSetup(pwmChannelRPWM, 1000, 8); // 1 kHz PWM, 8-bit resolution
  ledcSetup(pwmChannelLPWM, 1000, 8); // 1 kHz PWM, 8-bit resolution
  
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
  ledcWrite(pwmChannelRPWM, 255); // Full speed
  ledcWrite(pwmChannelLPWM, 0);   // Stop
  Serial.println("Motor running in one direction at full speed");
  delay(300); // Run motor for 0.3 seconds

  // Stop the motor
  ledcWrite(pwmChannelRPWM, 0);
  ledcWrite(pwmChannelLPWM, 0);
  Serial.println("Motor stopped");
  delay(500); // Stop for 0.5 second

  // Run motor in the opposite direction
  ledcWrite(pwmChannelRPWM, 0);
  ledcWrite(pwmChannelLPWM, 255); // Full speed
  Serial.println("Motor running in the opposite direction at full speed");
  delay(600); // Run motor for 0.6 seconds

  // Stop the motor
  ledcWrite(pwmChannelRPWM, 0);
  ledcWrite(pwmChannelLPWM, 0);
  Serial.println("Motor stopped");
  delay(1000); // Stop for 1 second

   // Run motor in one direction
  ledcWrite(pwmChannelRPWM, 255); // Full speed
  ledcWrite(pwmChannelLPWM, 0);   // Stop
  Serial.println("Motor running in one direction at full speed");
  delay(600); // Run motor for 0.6 seconds

  // Stop the motor
  ledcWrite(pwmChannelRPWM, 0);
  ledcWrite(pwmChannelLPWM, 0);
  Serial.println("Motor stopped");
  delay(500); // Stop for 0.5 second

  // Run motor in the opposite direction
  ledcWrite(pwmChannelRPWM, 0);
  ledcWrite(pwmChannelLPWM, 255); // Full speed
  Serial.println("Motor running in the opposite direction at full speed");
  delay(300); // Run motor for 0.3 seconds

  // Stop the motor
  ledcWrite(pwmChannelRPWM, 0);
  ledcWrite(pwmChannelLPWM, 0);
  Serial.println("Motor stopped");
  delay(1000); // Stop for 1 second
  }

  // Delay before uploading next set of data to ThingSpeak 
  // 300000 milliseconds = 5 minutes
  delay(300000);
}

