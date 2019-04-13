#include <Wire.h>
#include <MQ2.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(5, 6);

#define SEALEVELPRESSURE_HPA (1013.25)
#define PIN 2
#define NUM_SAMPLES 10

// number of analog samples to take per reading

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;
float v2 = 0;

//change this with the pin that you use
int pin = A3;
int lpg, co, smoke;
MQ2 mq2(pin);

int measurePin = A6;
int ledPower = 12;

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup() {

  s.begin(115200);
  Serial.begin(9600);
  pinMode(ledPower, OUTPUT);
}

StaticJsonBuffer<500> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void loop() {

  //  //---------------------DUST start---------------------
  digitalWrite(ledPower, LOW); // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin); // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH); // turn the LED off
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured * (3.3 / 1024);
  dustDensity = 0.17 * calcVoltage - 0.1;
  //Serial.println("dust");
  //Serial.println("bme");

  float* values = mq2.read(false); //set it false if you don't want to print the values in the Serial

  //lpg = values[0];
  lpg = mq2.readLPG();
  //co = values[1];
  co = mq2.readCO();
  //smoke = values[2];
  smoke = mq2.readSmoke();
  //Serial.println("gas");
  //---------------------GAS end-----------------------

  //Serial.println("check");

  while (sample_count < NUM_SAMPLES) {
    sum += analogRead(A2);
    sample_count++;
    delay(10);
  }
voltage = ((float)sum / (float)NUM_SAMPLES * 5.015) / 1024.0;


  v2 = voltage * 4.22;
  root["dust"] = dustDensity;
  root["lpg"] = lpg;
  root["co"] = co;
  root["smoke"] = smoke;
  root["voltage"] = v2;

  //root.printTo(Serial);
  root.printTo(s);
  //Serial.println("");

  sample_count = 0;
  sum = 0;
  //Serial.println("Send");
  delay(2000);
}
