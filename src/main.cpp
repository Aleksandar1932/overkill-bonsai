#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <Moisture.h>
#include <Constants.h>

ESP8266WebServer server(80);
ESP8266WiFiMulti wifiMulti;

void handleMeasurement();
void handleDisplayHTML();
void connectToWifi();
void handleDisplayPrettyHTML();

void setup()
{
  Serial.begin(9600);
  connectToWifi();
  setupSoilMoistureSensor();
  server.on("/", handleMeasurement);
  server.on("/display", handleDisplayHTML);
  server.begin();
}

void loop()
{
  int moisture = readSensor();
  logMoisture(moisture);
  delay(MEASURE_INTERVAL);
  server.handleClient();
}

void handleMeasurement()
{
  int moisture = readSensor();
  server.send(200, "application/json", "{\"moisture\": " + String(moisture) + ", \"status\": \"" + (moisture < WET_THRESHOLD ? "wet" : (moisture >= WET_THRESHOLD && moisture < DRY_TREHSHOLD ? "perfect" : "dry")) + "\"}");
}

void handleDisplayHTML()
{
  server.send(200, "text/html", "<html><head><title>ESP8266 Soil Moisture Sensor</title></head><body><h1>Aleksandar's Bonsai</h1><p>Moisture: " + String(readSensor()) + "</p></body></html>");
}

void connectToWifi()
{
  wifiMulti.addAP(getenv("WIFI_SSID"), getenv("WIFI_PASSWORD"));
  Serial.println("Connecting ...");

  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(getenv("MDNS_HOSTNAME")))
  {
    Serial.println("mDNS responder started");
  }
  else
  {
    Serial.println("Error setting up MDNS responder!");
  }
}
