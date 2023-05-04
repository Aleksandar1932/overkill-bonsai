#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

/* Change these values based on your calibration values */
#define WET_THRESHOLD 500     // Define max value we consider soil 'wet'
#define DRY_TREHSHOLD 750     // Define min value we consider soil 'dry'
#define MEASURE_INTERVAL 1000 // Define how often we check soil moisture (milliseconds)

// Sensor pins
#define sensorPower 0
#define sensorPin A0

int readSensor()
{
  digitalWrite(sensorPower, HIGH);
  delay(10);
  int val = analogRead(sensorPin);
  digitalWrite(sensorPower, LOW);
  return val;
}

void connectToWifi()
{
  const char *ssid = "Ivanovski WIFI";
  const char *password = "123ivanovski123";

  WiFi.mode(WIFI_AP); // Access Point mode
  WiFi.begin(ssid, password);
  Serial.println("Connecting to ");
  Serial.print(ssid);

  // Wait for WiFi to connect
  while (WiFi.status() == WL_CONNECTED)
  {
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupSoilMoistureSensor()
{
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
}

void setup()
{
  Serial.begin(9600);
  connectToWifi();
  setupSoilMoistureSensor();
}

void logMoisture(int moisture)
{
  Serial.print("Analog Output: ");
  Serial.println(moisture);

  // Determine status of our soil
  if (moisture < WET_THRESHOLD)
  {
    Serial.println("Status: Soil is too wet");
  }
  else if (moisture >= WET_THRESHOLD && moisture < DRY_TREHSHOLD)
  {
    Serial.println("Status: Soil moisture is perfect");
  }
  else
  {
    Serial.println("Status: Soil is too dry - time to water!");
  }
}

void loop()
{
  int moisture = readSensor();
  logMoisture(moisture);
  delay(MEASURE_INTERVAL);
}
