/*
Class: Agrotech Lab
Date: 24-07-2024
This code is designed to read soil moisture levels using a sensor and control a relay 
based on the moisture level. The relay can be used to control irrigation systems, 
turning them on when the soil is too dry and off when the soil has sufficient moisture.
*/

#define RELAY_PIN D12  // Define the relay pin as D12
#define SOIL_SENSOR_PIN A0  // Analog pin connected to the sensor

int dryValue = 0;  // Calibrate this value
int wetValue = 1023;  // Calibrate this value
int threshold = 40;  // Threshold moisture percentage to turn the relay on

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(115200);
  
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
  int sensorValue = analogRead(SOIL_SENSOR_PIN);
  
  // Map the sensor value to a percentage
 int moisturePercent = map(sensorValue, dryValue, wetValue, 0, 100);
  
  // Ensure the percentage is within 0-100%
  moisturePercent = constrain(moisturePercent, 0, 100);
  
  // Print the moisture percentage
  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

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
