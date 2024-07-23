# Soil Moisture Control Code

This code is designed to read soil moisture levels using a sensor and control a relay 
based on the moisture level. The relay can be used to control irrigation systems, 
turning them on when the soil is too dry and off when the soil has sufficient moisture.



## Pin Definitions and Global Variables
```cpp

#define RELAY_PIN D12  // Define the relay pin as D12. This pin controls the relay.
#define SOIL_SENSOR_PIN A0  // Analog pin connected to the soil moisture sensor.

int dryValue = 0;  // Initial dry calibration value; will be set during the setup.
int wetValue = 1023;  // Initial wet calibration value; will be set during the setup.
int threshold = 40;  // Threshold moisture percentage. Below this value, the relay will be turned on.
```

## Setup Function
```cpp
void setup() {
  pinMode(RELAY_PIN, OUTPUT);  // Set the relay pin as an output.
  Serial.begin(115200);  // Initialize serial communication at 115200 baud rate.
  
  Serial.println("Calibrating soil moisture sensor...");
  
  // Measure dry value
  // Prompt the user to insert the sensor into dry soil and press 'd'.
  Serial.println("Insert the sensor into dry soil and press 'd'");
  while (Serial.read() != 'd') {
    delay(100);  // Wait for the user to press 'd'.
  }
  dryValue = analogRead(SOIL_SENSOR_PIN);  // Read the sensor value in dry soil.
  Serial.print("Dry value set to: ");
  Serial.println(dryValue);
  
  // Measure wet value
  // Prompt the user to insert the sensor into wet soil and press 'w'.
  Serial.println("Insert the sensor into wet soil and press 'w'");
  while (Serial.read() != 'w') {
    delay(100);  // Wait for the user to press 'w'.
  }
  wetValue = analogRead(SOIL_SENSOR_PIN);  // Read the sensor value in wet soil.
  Serial.print("Wet value set to: ");
  Serial.println(wetValue);
  
  // Give some time before starting the loop
  delay(2000);  // Wait for 2 seconds before starting the loop.
}
```
## Loop Function
```cpp
void loop() {
  int sensorValue = analogRead(SOIL_SENSOR_PIN);  // Read the current soil moisture value from the sensor.
  
  // Map the sensor value to a moisture percentage.
  int moisturePercent = map(sensorValue, dryValue, wetValue, 0, 100);
  
  // Ensure the percentage is within 0-100%.
  moisturePercent = constrain(moisturePercent, 0, 100);
  
  // Print the moisture percentage to the serial monitor.
  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  // Control the relay based on soil moisture
  if (moisturePercent < threshold) {
    digitalWrite(RELAY_PIN, HIGH);  // Turn the relay on (this typically means the relay will open the connected circuit).
    Serial.println("Relay ON");
  } else {
    digitalWrite(RELAY_PIN, LOW);  // Turn the relay off (this typically means the relay will close the connected circuit).
    Serial.println("Relay OFF");
  }

  delay(1000);  // Wait for 1 second before taking another reading.
}
```
