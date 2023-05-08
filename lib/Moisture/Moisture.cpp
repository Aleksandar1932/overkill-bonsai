#include <Arduino.h>
#include <Constants.h>

int readSensor()
{
    digitalWrite(sensorPower, HIGH);
    delay(10);
    int val = analogRead(sensorPin);
    digitalWrite(sensorPower, LOW);
    return val;
}


void setupSoilMoistureSensor()
{
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
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