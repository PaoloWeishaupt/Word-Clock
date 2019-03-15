#include <Wire.h> //libreria per interfacciare i2c e rtc
#include "RTClib.h" //libreria rtc
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6

RTC_DS1307 rtc;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(195, PIN, NEO_GRB + NEO_KHZ800);

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
  strip.setBrightness(255);
  strip.show();
  if (! rtc.begin()) { //verifico la presenza dell'RTC
    Serial.println("Impossibile trovare RTC");
    while (1);
  }

  if (! rtc.isrunning()) { //verifico funzionamento dell'RTC
    Serial.println("RTC non è in funzione!");
    //inserisce l'orario del computer durante la compilazione
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Se vuoi un orario personalizzato, togli il commento alla riga successiva
    // l'orario: ANNO, MESE, GIORNI, ORA, MINUTI, SECONDI
    //rtc.adjust(DateTime(2014, 1, 12, 18, 0, 0));
  }
  rtc.adjust(DateTime(2014, 1, 12, 15, 0, 0));
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now(); //creo istanza ora/data
  int hour = now.hour(); //DA SISTEMARE
  int minute = now.minute();
  int second = now.second();
  Serial.print(now.year(), DEC); //stampo anno in decimale
  Serial.print('/');
  Serial.print(now.month(), DEC); //stampo mese in decimale
  Serial.print('/');
  Serial.print(now.day(), DEC); //stampo giorno in decimale
  Serial.print(" (");
  Serial.print(now.hour(), DEC); //stampo ora in decimale
  Serial.print(':');
  Serial.print(now.minute(), DEC); //stampo minuto in decimale
  Serial.print(':');
  Serial.print(now.second(), DEC); //stampo secondi in decimale
  Serial.println();
  for(int i = 1; i < 13; i++){
      generateWord(i, 0, 0, black);
  }
  printTime(hour, minute, second);
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
  if ((hour != 1 && hour != 13) || minute >= 35) {
    generateWord(1, 2, 5, red);
    generateWord(1, 7, 8, red);
    //time += "Sono le ";
    if (hour == 2 || hour == 14) {
      generateWord(2, 12, 14, red);
      //time += "due ";
    } else if (hour == 3 || hour == 15) {
      //tre();
      generateWord(3, 1, 3, red);
      //time += "tre ";
    } else if (hour == 4 || hour == 16) {
      generateWord(3, 5, 11, red);
      //time += "quattro ";
    } else if (hour == 5 || hour == 17) {
      generateWord(4, 1, 6, red);
      //time += "cinque ";
    } else if (hour == 6 || hour == 18) {
      generateWord(3, 12, 14, red);
      //time += "sei ";
    } else if (hour == 7 || hour == 19) {
      generateWord(4, 10, 14, red);
      //time += "sette ";
    } else if (hour == 8 || hour == 20) {
      generateWord(5, 1, 4, red);
      //time += "otto ";
    } else if (hour == 9 || hour == 21) {
      generateWord(5, 5, 8, red);
      //time += "nove ";
    } else if (hour == 10 || hour == 22) {
      generateWord(5, 10, 14, red);
      //time += "dieci ";
    } else if (hour == 11 || hour == 23) {
      generateWord(6, 1, 6, red);
      //time += "undici ";
    } else if (hour == 12) {
      generateWord(2, 1, 11, red);
      //time += "mezzogiorno ";
    } else if (hour == 24) {
      generateWord(7, 1, 10, red);
      //time += "mezzanotte ";
    }
  } else {
    generateWord(1, 1, 1, red);
    generateWord(1, 10, 14, red);
    //time += "È l'una";
  }
  //secondi
  if(second >= 0 && second <= 4){
    generateWord(1, 0, 0, red);
  }
  else if(second >= 5 && second <= 9){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
  }
  else if(second >= 10 && second <= 14){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
    generateWord(3, 0, 0, red);
  }
  else if(second >= 15 && second <= 19){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
    generateWord(3, 0, 0, red);
    generateWord(4, 0, 0, red);
  }
  else if(second >= 20 && second <= 24){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
    generateWord(3, 0, 0, red);
    generateWord(4, 0, 0, red);
    generateWord(5, 0, 0, red);
  }
  else if(second >= 25 && second <= 29){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
    generateWord(3, 0, 0, red);
    generateWord(4, 0, 0, red);
    generateWord(5, 0, 0, red);
    generateWord(6, 0, 0, red);
  }
  else if(second >= 30 && second <= 34){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
    generateWord(3, 0, 0, red);
    generateWord(4, 0, 0, red);
    generateWord(5, 0, 0, red);
    generateWord(6, 0, 0, red);
    generateWord(7, 0, 0, red);
  }
  else if(second >= 35 && second <= 39){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
    generateWord(3, 0, 0, red);
    generateWord(4, 0, 0, red);
    generateWord(5, 0, 0, red);
    generateWord(6, 0, 0, red);
    generateWord(7, 0, 0, red);
    generateWord(8, 0, 0, red);
  }
  else if(second >= 40 && second <= 44){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
    generateWord(3, 0, 0, red);
    generateWord(4, 0, 0, red);
    generateWord(5, 0, 0, red);
    generateWord(6, 0, 0, red);
    generateWord(7, 0, 0, red);
    generateWord(8, 0, 0, red);
    generateWord(9, 0, 0, red);
  }
  else if(second >= 45 && second <= 49){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
    generateWord(3, 0, 0, red);
    generateWord(4, 0, 0, red);
    generateWord(5, 0, 0, red);
    generateWord(6, 0, 0, red);
    generateWord(7, 0, 0, red);
    generateWord(8, 0, 0, red);
    generateWord(9, 0, 0, red);
    generateWord(10, 0, 0, red);
  }
  else if(second >= 50 && second <= 54){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
    generateWord(3, 0, 0, red);
    generateWord(4, 0, 0, red);
    generateWord(5, 0, 0, red);
    generateWord(6, 0, 0, red);
    generateWord(7, 0, 0, red);
    generateWord(8, 0, 0, red);
    generateWord(9, 0, 0, red);
    generateWord(10, 0, 0, red);
    generateWord(11, 0, 0, red);
  }
  else if(second >= 55 && second <= 59){
    generateWord(1, 0, 0, red);
    generateWord(2, 0, 0, red);
    generateWord(3, 0, 0, red);
    generateWord(4, 0, 0, red);
    generateWord(5, 0, 0, red);
    generateWord(6, 0, 0, red);
    generateWord(7, 0, 0, red);
    generateWord(8, 0, 0, red);
    generateWord(9, 0, 0, red);
    generateWord(10, 0, 0, red);
    generateWord(11, 0, 0, red);
    generateWord(12, 0, 0, red);
  }
}

void generateWord(int row, int min, int max, uint32_t color) {
  Serial.println(row);
  Serial.println(min);
  Serial.println(max);
  Serial.println(color);
  if(min == 0 && max == 0){
    for (int i = min; i <= (max - min); i++) {
      pixelOn(pixels[row][i], color);
    }
  }
  else{
    for (int i = min; i <= (max - min) + 1; i++) {
      pixelOn(pixels[row][i], color);
    }
  }
}
