#include <SPI.h>
#include <SD.h>

// =====================================
// PIN CONFIGURATION
// =====================================

// HC-SR04 Ultrasonic Sensor
const int TRIG_PIN = 7;
const int ECHO_PIN = 6;

// MicroSD Module
const int SD_CS_PIN = 10;

// =====================================
// DATA LOGGING SETTINGS
// =====================================

const unsigned long INTERVAL = 500;
const unsigned long TIMEOUT = 30000;

const char* FILE_NAME = "distance.csv";

// =====================================
// VARIABLES
// =====================================

File dataFile;

unsigned long previousMillis = 0;
unsigned long startTime = 0;

// =====================================
// SETUP
// =====================================

void setup() {

  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  delay(1000);

  Serial.println("Arduino is connected!");
  Serial.println("Serial communication is working.");

  Serial.println("================================");
  Serial.println("   ULTRASONIC DATA LOGGER");
  Serial.println("================================");

  // Initialize SD card
  Serial.println("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {

    Serial.println("ERROR: SD card initialization failed!");

    while (true) {
      delay(1000);
    }
  }

  Serial.println("SD card initialized successfully!");

  // Create or open CSV file
  dataFile = SD.open(FILE_NAME, FILE_WRITE);

  if (!dataFile) {

    Serial.println("ERROR: Cannot open CSV file!");

    while (true) {
      delay(1000);
    }
  }

  // Write headers if file is empty
  if (dataFile.size() == 0) {

    dataFile.println("Time (ms), Distance (cm)");

  }

  dataFile.flush();
  dataFile.close();

  Serial.println("CSV file ready!");
  Serial.println("Starting measurements...");
  Serial.println("--------------------------------");

  startTime = millis();
  previousMillis = millis();
}

// =====================================
// MAIN LOOP
// =====================================

void loop() {

  Serial.println("Hello from Neil, Jacksonthegoat and Jack");
  delay(1000);

  unsigned long currentMillis = millis();

  // Check sampling interval
  if (currentMillis - previousMillis >= INTERVAL) {

    previousMillis += INTERVAL;

    // Trigger ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(500);

    digitalWrite(TRIG_PIN, LOW);

    // Read echo duration
    unsigned long duration =
      pulseIn(ECHO_PIN, HIGH, TIMEOUT);

    // Calculate distance
    float distance = -1;

    if (duration > 0) {

      distance = duration * 0.0343 / 2.0;

    }

    // Calculate elapsed time
    unsigned long timestamp = millis() - startTime;

    // Display results
    Serial.print("Time: ");
    Serial.print(timestamp);
    Serial.print(" ms | Distance: ");

    if (distance >= 0) {

      Serial.print(distance, 2);
      Serial.println(" cm");

    } else {

      Serial.println("No echo");

    }

    // Open CSV file
    dataFile = SD.open(FILE_NAME, FILE_WRITE);

    if (dataFile) {

      // Save timestamp
      dataFile.print(timestamp);
      dataFile.print(",");

      // Save distance
      if (distance >= 0) {

        dataFile.println(distance, 2);

      } else {

        dataFile.println("No echo");

      }

      // Save and close file
      dataFile.flush();
      dataFile.close();

    } else {

      Serial.println("ERROR: SD card write failed!");

    }
  }
}
