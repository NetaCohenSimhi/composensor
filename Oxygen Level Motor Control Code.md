
# Oxygen Level Motor Control Code
This code controls a motor based on oxygen levels and sends the data to ThingSpeak.
It connects to a WiFi network, reads (simulated) oxygen levels, controls the motor accordingly,
and uploads the data to the ThingSpeak platform for monitoring.



## Include Necessary Libraries
```cpp
#include <Arduino.h>
#include <Wire.h>
#include <ThingSpeak.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h> 

```
## ThingSpeak Configuration
```cpp
unsigned long myChannelNumber = 2591283;
const char *myWriteAPIKey = "84QHA0LT3CZ25OK3";
const char *ssid = "agrotech"; // WiFi SSID name
const char *password = "1Afuna2gezer"; // WiFi password
```

## Motor Control Pins
```cpp
const int RPWM = D9;  // Right PWM
const int LPWM = D10;  // Left PWM
const int REN = D11;  // Right enable
const int LEN = D11;  // Left enable
```

## Define PWM Channels
```cpp
const int pwmChannelRPWM = 0;
const int pwmChannelLPWM = 1;

```
## Create WiFi Client Object
```cpp
WiFiClient client;
```

## Function to Generate Random Oxygen Level
```cpp
// Generates a random number between 0 and 21 inclusive to simulate oxygen level
float generateRandomOxygenLevel() {
  return random(0, 21); 
}
```

## Setup Function
```cpp
// The setup function runs once at the beginning
void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate

  // Disconnect and connect to WiFi
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
```
## Loop Function
```cpp
// The loop function runs continuously after setup
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
```

[Click to get back to the first page](https://github.com/NetaCohenSimhi/composensor/blob/main/README.md)
