#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT11 setup
#define DHT_PIN 4
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// MQ-2 setup
#define MQ2_ANALOG_PIN 34   // GPIO34 for analog reading
#define MQ2_DIGITAL_PIN 35  // GPIO35 for digital reading (optional)

// PIR Motion Sensor setup
#define PIR_PIN 32          // GPIO32 for PIR sensor

// Relay setup (ACTIVE LOW)
#define RELAY_PIN 13        // GPIO33 for relay control

// LED setup for motion indicator
#define LED_PIN 25          // GPIO25 for LED (you can change this pin as needed)

// Variables
float temperatureReading, humidityReading;
int gasSensorValue;
bool motionDetected = false;
bool previousMotionState = false;
unsigned long previousMillis = 0;
unsigned long motionDisplayStart = 0;
const long displayInterval = 5000; // 5 seconds
const long motionDisplayDuration = 10000; // Show motion for 10 seconds
int displayMode = 0; // 0: Temp/Humidity, 1: Gas Level, 2: Motion Status
bool showingMotionAlert = false;

void setup() {
  Serial.begin(115200);
  
  // Initialize sensors
  dht.begin();
  pinMode(MQ2_DIGITAL_PIN, INPUT);
  pinMode(PIR_PIN, INPUT_PULLUP);  // Enable internal pull-up for PIR
  
  // Initialize relay with INTERNAL PULL-UP (ACTIVE LOW)
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Start with relay OFF (HIGH for active low)
  
  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Start with LED off
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Show initialization sequence
  showInitSequence();
}

void showInitSequence() {
  // Step 1: Show system initialization
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing");
  lcd.setCursor(0, 1);
  lcd.print("Maize Storage");
  delay(2500);
  
  // Step 2: Show management system
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Storage Mgmt");
  lcd.setCursor(0, 1);
  lcd.print("System");
  delay(2500);
  
  // Step 3: Show sensors status
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting");
  lcd.setCursor(0, 1);
  lcd.print("Sensors...");
  delay(2000);
  
  // Step 4: Show ready message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring...");
  delay(2000);
}

void readAllSensors() {
  // Read DHT11 Temperature and Humidity
  temperatureReading = dht.readTemperature();
  humidityReading = dht.readHumidity();
  
  // Read MQ-2 Gas Sensor
  gasSensorValue = analogRead(MQ2_ANALOG_PIN);
  
  // Read PIR Motion Sensor (with pull-up, so motion is typically LOW)
  bool currentMotion = digitalRead(PIR_PIN);
  
  // Note: With INPUT_PULLUP, the logic may be inverted depending on your PIR sensor
  // Most PIR sensors output HIGH when motion detected, but with pull-up it might be different
  // You may need to adjust this logic based on your actual PIR sensor behavior
  
  // Check if motion state changed from no motion to motion
  if (currentMotion && !previousMotionState) {
    motionDetected = true;
    showingMotionAlert = true;
    motionDisplayStart = millis();
    
    // TURN OFF RELAY (LOW) AND TURN ON LED WHEN MOTION IS DETECTED
    digitalWrite(RELAY_PIN, LOW);  // LOW turns OFF the device (active low with pull-up)
    digitalWrite(LED_PIN, HIGH);   // Turn LED ON
    Serial.println("MOTION DETECTED - Relay turned OFF! LED turned ON!");
    
  } else if (!currentMotion && previousMotionState) {
    motionDetected = false;
    
    // TURN ON RELAY (HIGH) AND TURN OFF LED WHEN MOTION STOPS
    digitalWrite(RELAY_PIN, HIGH); // HIGH turns ON the device again
    digitalWrite(LED_PIN, LOW);    // Turn LED OFF
    Serial.println("Motion stopped - Relay turned ON, LED turned OFF");
  }
  
  previousMotionState = currentMotion;
  
  // Print to Serial Monitor for debugging
  Serial.print("Temperature: ");
  Serial.print(temperatureReading);
  Serial.print("°C, Humidity: ");
  Serial.print(humidityReading);
  Serial.print("%, Gas Sensor: ");
  Serial.print(gasSensorValue);
  Serial.print(", Motion: ");
  Serial.print(motionDetected ? "DETECTED" : "NO MOTION");
  Serial.print(", PIR Raw: ");
  Serial.print(digitalRead(PIR_PIN));
  Serial.print(", Relay: ");
  Serial.print(digitalRead(RELAY_PIN) ? "ON" : "OFF"); // Adjusted logic
  Serial.print(", LED: ");
  Serial.println(digitalRead(LED_PIN) ? "ON" : "OFF");
}

void displayTempHumidity() {
  lcd.clear();
  
  // Line 1: Temperature
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  if (!isnan(temperatureReading)) {
    lcd.setCursor(0, 1);
    lcd.print(temperatureReading, 1);
    lcd.print(" C");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Error");
  }
  
  // Line 2: Humidity (on the right side)
  lcd.setCursor(9, 0);
  lcd.print("Hum:");
  if (!isnan(humidityReading)) {
    lcd.setCursor(9, 1);
    lcd.print(humidityReading, 1);
    lcd.print(" %");
  } else {
    lcd.setCursor(9, 1);
    lcd.print("Error");
  }
}

void displayGasLevel() {
  lcd.clear();
  
  // Line 1: Header
  lcd.setCursor(0, 0);
  lcd.print("Gas Level");
  
  // Line 2: Gas sensor value (centered)
  lcd.setCursor(0, 1);
  lcd.print("Value: ");
  lcd.print(gasSensorValue);
  
  // Add gas level interpretation
  lcd.setCursor(10, 1);
  if (gasSensorValue < 1000) {
    lcd.print("SAFE");
  } else if (gasSensorValue < 2000) {
    lcd.print("MED");
  } else {
    lcd.print("HIGH");
  }
}

void displayMotionStatus() {
  lcd.clear();
  
  // Line 1: Header
  lcd.setCursor(0, 0);
  lcd.print("Motion Sensor");
  
  // Line 2: Motion status
  lcd.setCursor(0, 1);
  lcd.print("Status: ");
  
  if (motionDetected) {
    lcd.print("DETECTED");
    lcd.setCursor(15, 1);
    lcd.print("!");
  } else {
    lcd.print("NO PEST");
    lcd.setCursor(15, 1);
    lcd.print(" ");
  }
}

void displayMotionAlert() {
  lcd.clear();
  
  // Line 1: Warning message
  lcd.setCursor(0, 0);
  lcd.print("Fumigating");
  
  // Line 2: Motion detected message
  lcd.setCursor(0, 1);
  lcd.print("PEST DETECTED!");
  
  // Optional: Make it blink for more attention
  static unsigned long lastBlink = 0;
  static bool blinkState = false;
  
  if (millis() - lastBlink > 500) { // Blink every 500ms
    lastBlink = millis();
    blinkState = !blinkState;
    lcd.setBacklight(blinkState ? 255 : 0); // Blink backlight
  }
}

void loop() {
  // Read all sensors
  readAllSensors();
  
  unsigned long currentMillis = millis();
  
  // Check if we're currently showing a motion alert
  if (showingMotionAlert) {
    displayMotionAlert();
    
    // Check if motion alert duration has elapsed
    if (currentMillis - motionDisplayStart >= motionDisplayDuration) {
      showingMotionAlert = false;
      lcd.setBacklight(255); // Ensure backlight is on
      // Reset the display timer to avoid immediate switch
      previousMillis = currentMillis;
    }
  } else {
    // Normal display cycling when no motion alert
    if (currentMillis - previousMillis >= displayInterval) {
      previousMillis = currentMillis;
      displayMode = (displayMode + 1) % 3;
    }
    
    // Display the appropriate reading based on current mode
    switch (displayMode) {
      case 0:
        displayTempHumidity();
        break;
      case 1:
        displayGasLevel();
        break;
      case 2:
        displayMotionStatus();
        break;
    }
  }
  
  delay(1000); // Update every second
} 