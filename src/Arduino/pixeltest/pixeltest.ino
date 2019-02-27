#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(195, PIN, NEO_GRB + NEO_KHZ800);

int matrix[15][13] = {
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
  {25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13},
  {26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38},
  {51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39},
  {52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64},
  {77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65},
  {78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90},
  {103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91},
  {104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116},
  {129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117},
  {130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142},
  {155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143},
  {156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168},
  {181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169},
  {182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194},
};

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(100);
  strip.show();
}

void loop() {
  strip.setPixelColor(matrix[10][10], 255, 255, 255);
  strip.show();
  delay(5000);
  colorWipe(255, 0, 0);
  colorWipe(0, 255, 0);
  colorWipe(0, 0, 255);
  whitePixel();
  //colorWipe(255, 255, 255);

}

void colorWipe(int r, int g, int b) {
  int i = 0;
  while (i < strip.numPixels()) {
    strip.setPixelColor(i, r, g, b);
    strip.show();
    delay(5);
    i++;
  }
}

void whitePixel() {
  int i = 0;
  while (i < strip.numPixels()) {
    strip.setPixelColor(i, 255, 255, 255);
    strip.show();
    strip.setPixelColor(i, 0, 0, 0);
    delay(5);
    i++;
  }
}
