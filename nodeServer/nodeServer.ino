#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define SEALEVELPRESSURE_HPA (1013.25)

const char* ssid = "Vratsa Software - ConferenceRoom";
const char* password = "thisIsYourMoment!";

ESP8266WebServer server(80);
String page = "";
String page2 = "";
Adafruit_BME280 bme;
float temp = 0;
float pressure = 0;
float alt = 0;
float humi = 0;

void setup() {
  Serial.begin(9600);
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
  //  server.on("/", [](){
  //    page = "<h1>Sensor to Node MCU Web Server</h1><h3>Data:</h3> <h4>"+String(temp)+"</h4>";
  //    server.send(200, "text/html", page);
  //  });

  server.begin();
  Serial.println("Web server started!");
}

void loop() {

  temp = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humi = bme.readHumidity();

  //  Serial.print("Temperature = ");
  //  Serial.print(temp);
  //  Serial.println("*C");
  //
  //  Serial.print("Pressure = ");
  //  Serial.print(pressure);
  //  Serial.println("hPa");
  //
  //  Serial.print("Approx. Altitude = ");
  //  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  //  Serial.println("m");
  //
  //  Serial.print("Humidity = ");
  //  Serial.print(bme.readHumidity());
  //  Serial.println("%");

  //Serial.println();
  delay(1000);
  server.on("/", []() {
    page = "Temperature: " + String(temp) + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pressure: " + String(pressure) + "<br>Aprox. Altitude: " + String(alt) + "&nbsp; &nbsp;Humidity: " + String(humi) + "";
    server.send(200, "text/html", page);
  });
  server.handleClient();
}
