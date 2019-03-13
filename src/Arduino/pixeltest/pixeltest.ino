#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(195, PIN, NEO_GRB + NEO_KHZ800);
/*
  const int pixels[15][13] = {
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
*/

const int pixels[13][15] = {
  {182, 181, 156, 155, 130, 129, 104, 103, 78, 77, 52, 51, 26, 25, 0},
  {183, 180, 157, 154, 131, 128, 105, 102, 79, 76, 53, 50, 27, 24, 1},
  {184, 179, 158, 153, 132, 127, 106, 101, 80, 75, 54, 49, 28, 23, 2},
  {185, 178, 159, 152, 133, 126, 107, 100, 81, 74, 55, 48, 29, 22, 3},
  {186, 177, 160, 151, 134, 125, 108, 99, 82, 73, 56, 47, 30, 21, 4},
  {187, 176, 161, 150, 135, 124, 109, 98, 83, 72, 57, 46, 31, 20, 5},
  {188, 175, 162, 149, 136, 123, 110, 97, 84, 71, 58, 45, 32, 19, 6},
  {189, 174, 163, 148, 137, 122, 111, 96, 85, 70, 59, 44, 33, 18, 7},
  {190, 173, 164, 147, 138, 121, 112, 95, 86, 69, 60, 43, 34, 17, 8},
  {191, 172, 165, 146, 139, 120, 113, 94, 87, 68, 61, 42, 35, 16, 9},
  {192, 171, 166, 145, 140, 119, 114, 93, 88, 67, 62, 41, 36, 15, 10},
  {193, 170, 167, 144, 141, 118, 115, 92, 89, 66, 63, 40, 37, 14, 11},
  {194, 169, 168, 143, 142, 117, 116, 91, 90, 65, 64, 39, 38, 13, 12}
};

const uint32_t red = strip.Color(255, 0, 0);
const uint32_t green = strip.Color(0, 255, 0);
const uint32_t blue = strip.Color(0, 0, 255);
const uint32_t white = strip.Color(255, 255, 255);
const uint32_t black = strip.Color(0, 0, 0);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(100);
  strip.show();
}

void loop() {

}

void pixelOn(int pixel, uint32_t color) {
  strip.setPixelColor(pixel, color);
  strip.show();
}

void colorWipe(uint32_t color) {
  int i = 0;
  while (i < strip.numPixels()) {
    pixelOn(i, color);
    delay(5);
    i++;
  }
}

void whitePixel() {
  int i = 0;
  while (i < strip.numPixels()) {
    pixelOn(i, white);
    pixelOn(i, black);
    delay(5);
    i++;
  }
}

void ciao() {
  for (int i = 0; i < 3; i++) {
    pixelOn(pixels[3][i], white);
    pixelOn(pixels[3 + i][0], white);
    pixelOn(pixels[5][i], white);
    pixelOn(pixels[3 + i][4], blue);
    pixelOn(pixels[4][6 + i], green);
    pixelOn(pixels[3 + i][6], green);
    pixelOn(pixels[3 + i][8], green);
    pixelOn(pixels[2][7], green);
    pixelOn(pixels[3][10 + i], red);
    pixelOn(pixels[3 + i][10], red);
    pixelOn(pixels[5][10 + i], red);
    pixelOn(pixels[3 + i][12], red);
  }
}

void randPixel() {
  uint32_t c = strip.Color(random(100), random(100), random(100));
  int randI = random(13);
  int randJ = random(15);
  pixelOn(pixels[randI][randJ], c);
}

void printTime(int hour, int minute, int second) {
  if (hour != 1 && hour != 13) {
    pixelOn(pixels[1][2], red);
    pixelOn(pixels[1][3], red);
    pixelOn(pixels[1][4], red);
    pixelOn(pixels[1][5], red);
    pixelOn(pixels[1][7], red);
    pixelOn(pixels[1][8], red);
    //time += "Sono le ";
    if (hour == 2 || hour == 14) {
      pixelOn(pixels[2][12], red);
      pixelOn(pixels[2][13], red);
      pixelOn(pixels[2][14], red);
      //time += "due ";
    } else if (hour == 3 || hour == 15) {
      pixelOn(pixels[3][1], red);
      pixelOn(pixels[3][2], red);
      pixelOn(pixels[3][3], red);
      //time += "tre ";
    } else if (hour == 4 || hour == 16) {
      pixelOn(pixels[3][5], red);
      pixelOn(pixels[3][6], red);
      pixelOn(pixels[3][7], red);
      pixelOn(pixels[3][8], red);
      pixelOn(pixels[3][9], red);
      pixelOn(pixels[3][10], red);
      pixelOn(pixels[3][11], red);
      //time += "quattro ";
    } else if (hour == 5 || hour == 17) {
      pixelOn(pixels[4][1], red);
      pixelOn(pixels[4][2], red);
      pixelOn(pixels[4][3], red);
      pixelOn(pixels[4][4], red);
      pixelOn(pixels[4][5], red);
      pixelOn(pixels[4][6], red);
      //time += "cinque ";
    } else if (hour == 6 || hour == 18) {
      pixelOn(pixels[3][12], red);
      pixelOn(pixels[3][13], red);
      pixelOn(pixels[3][14], red);
      //time += "sei ";
    } else if (hour == 7 || hour == 19) {
      pixelOn(pixels[4][10], red);
      pixelOn(pixels[4][11], red);
      pixelOn(pixels[4][12], red);
      pixelOn(pixels[4][13], red);
      pixelOn(pixels[4][14], red);
      //time += "sette ";
    } else if (hour == 8 || hour == 20) {
      pixelOn(pixels[2][12], red);
      pixelOn(pixels[2][13], red);
      pixelOn(pixels[2][14], red);
      pixelOn(pixels[2][12], red);
      //time += "otto ";
    } else if (hour == 9 || hour == 21) {
      pixelOn(pixels[2][12], red);
      pixelOn(pixels[2][13], red);
      pixelOn(pixels[2][14], red);
      pixelOn(pixels[2][12], red);
      //time += "nove ";
    } else if (hour == 10 || hour == 22) {
      pixelOn(pixels[2][12], red);
      pixelOn(pixels[2][13], red);
      pixelOn(pixels[2][14], red);
      pixelOn(pixels[2][12], red);
      pixelOn(pixels[2][13], red);
      //time += "dieci ";
    } else if (hour == 11 || hour == 23) {
      pixelOn(pixels[2][12], red);
      pixelOn(pixels[2][13], red);
      pixelOn(pixels[2][14], red);
      pixelOn(pixels[2][12], red);
      pixelOn(pixels[2][13], red);
      pixelOn(pixels[2][14], red);
      //time += "undici ";
    } else if (hour == 12 || hour == 24) {
      pixelOn(pixels[2][12], red);
      pixelOn(pixels[2][13], red);
      pixelOn(pixels[2][14], red);
      pixelOn(pixels[2][12], red);
      pixelOn(pixels[2][13], red);
      pixelOn(pixels[2][14], red);
      //time += "dodici ";
    }
  } else {
    //time += "È la una";
  }
  //return time;
}
