//include some useful libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//calibrate SEA_LEVEL
#define SEALEVELPRESSURE_HPA (1013.25)

const char* ssid = "Vratsa Software - ConferenceRoom";
const char* password = "thisIsYourMoment!";

String page = "";
float temp = 0;
float pressure = 0;
float alt = 0;
float humi = 0;
float val1;
float val2;
float val3;
float val4;
float val5;

//start services
ESP8266WebServer server(80);
SoftwareSerial s(D6, D5);
Adafruit_BME280 bme;

void setup() {
  //setting up communication interfaces
  Serial.begin(115200);
  s.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
  {
    return;
  }

  //-----bme280_READ------
  temp = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humi = bme.readHumidity();
  //-----bme280_END------

  //---extract values-----
  val1 = root["dust"];
  val2 = root["lpg"];
  val3 = root["co"];
  val4 = root["smoke"];
  val5 = root["voltage"];

  //--extract values END---

  //-----WEB PAGE START-----
  server.on("/", []() {
    page = "Temperature: " + String(temp) + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pressure: " + String(pressure) + "<br>Aprox. Altitude: " + String(alt) + "&nbsp; &nbsp;Humidity: " + String(humi) + "<br>Dust Density: " + String(val1) + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;LPG: " + String(val2) + "<br>" + "CO: " + String(val3) + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Smoke: " + String(val4) + "<br>Voltage: " + String(val5) + "" ;
    server.send(200, "text/html", page);
  });
  server.handleClient();

  //-----WEB PAGE END------
}
