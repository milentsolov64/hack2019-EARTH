#include <Adafruit_NeoPixel.h>

#define PIN 2

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop()
{

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

  pixels.setPixelColor(0, pixels.Color(255, 0, 0)); //blue, red, green
  pixels.show(); // This sends the updated pixel color to the hardware.

}
