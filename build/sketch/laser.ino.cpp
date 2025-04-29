#include <Arduino.h>
#line 1 "C:\\Users\\kavee\\Desktop\\MECH3890\\VSCODE\\Experiments\\laser\\laser.ino"
#include "Adafruit_VL53L1X.h"
#include <Wire.h>

// Define pins for the VL53L1X sensor
#define IRQ_PIN 2
#define XSHUT_PIN 3

// Create a VL53L1X sensor object
Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);

// Function to initialize the sensor
#line 12 "C:\\Users\\kavee\\Desktop\\MECH3890\\VSCODE\\Experiments\\laser\\laser.ino"
int initializeSensor(int sdaPin, int sclPin);
#line 29 "C:\\Users\\kavee\\Desktop\\MECH3890\\VSCODE\\Experiments\\laser\\laser.ino"
int getDistance();
#line 43 "C:\\Users\\kavee\\Desktop\\MECH3890\\VSCODE\\Experiments\\laser\\laser.ino"
void setup();
#line 57 "C:\\Users\\kavee\\Desktop\\MECH3890\\VSCODE\\Experiments\\laser\\laser.ino"
void loop();
#line 12 "C:\\Users\\kavee\\Desktop\\MECH3890\\VSCODE\\Experiments\\laser\\laser.ino"
int initializeSensor(int sdaPin, int sclPin) {
  Wire.begin(sdaPin, sclPin); // Initialize I2C on custom SDA/SCL pins

  if (!vl53.begin(0x29, &Wire)) {
    return -1; // Sensor initialization failed
  }

  // Start ranging mode
  if (!vl53.startRanging()) {
    return -1; // Sensor failed to start ranging
  }

  vl53.setTimingBudget(50); // Set timing budget to 50ms
  return 0; // Success
}

// Function to measure distance
int getDistance() {
  if (!vl53.dataReady()) {
    return -2; // Distance measurement not ready
  }

  int16_t distance = vl53.distance();
  if (distance == -1) {
    return -1; // Sensor read failure
  }

  vl53.clearInterrupt(); // Clear interrupt for the next measurement
  return distance; // Return the valid distance in mm
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for Serial connection

  // Initialize the sensor on SDA = GPIO 4 and SCL = GPIO 5
  if (initializeSensor(4, 5) == -1) {
    // If initialization fails, halt without printing additional text
    while (1) delay(10);
  }

  // Print CSV header (optional)
  Serial.println("timestamp,distance");
}

void loop() {
  int distance = getDistance();
  unsigned long timestamp = millis();  // Capture current timestamp (ms)

  // Output CSV row: timestamp and distance (in mm)
  Serial.print(timestamp);
  Serial.print(",");
  Serial.println(distance);

  delay(100); // Delay between readings
}

