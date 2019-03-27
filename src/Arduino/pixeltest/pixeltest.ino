#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(195, PIN, NEO_GRB + NEO_KHZ800);

// Matrice utile per identificare i led.
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

// Valori che definiscono i colori principali.
const uint32_t red = strip.Color(255, 0, 0);
const uint32_t green = strip.Color(0, 255, 0);
const uint32_t blue = strip.Color(0, 0, 255);
const uint32_t white = strip.Color(255, 255, 255);
const uint32_t black = strip.Color(0, 0, 0);

/**
   Impostazione del dispositivo.
*/
void setup() {
  strip.begin();
  strip.setBrightness(255);
  strip.show();
}

/**
   Esecuzione delle operazioni in ciclo continuo.
*/
void loop() {
  pixelOn(pixels[10][10], white);
  delay(3000);
  colorWipe(red);
  colorWipe(green);
  colorWipe(blue);
  whitePixel();
  ciao();
  delay(3000);
  randPixel();
}

/**
   Accensione di un led di un colore definito.
*/
void pixelOn(int pixel, uint32_t color) {
  strip.setPixelColor(pixel, color);
  strip.show();
}

/**
   Accensione di tutti i led uno dopo l'altro.
*/
void colorWipe(uint32_t color) {
  int i = 0;
  while (i < strip.numPixels()) {
    pixelOn(i, color);
    delay(5);
    i++;
  }
}

/**
   Accensione di ogni singolo led di bianco in successione.
*/
void whitePixel() {
  int i = 0;
  while (i < strip.numPixels()) {
    pixelOn(i, white);
    pixelOn(i, black);
    delay(5);
    i++;
  }
}

/**
   Accensione di led che formano la scritta "CIAO".
*/
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

/**
   Accensione di un led casuale di un colore casuale.
*/
void randPixel() {
  uint32_t color = strip.Color(random(100), random(100), random(100));
  pixelOn(pixels[random(13)][random(15)], color);
}
