/*
Word Clock
Gabriele Alessi, Mattia Lazzaroni, Paolo Claudio Weishaupt
https://github.com/PaoloWeishaupt/Word-Clock
*/

#include <SPI.h>
#include <Fishino.h>
#define DEBUG_LEVEL_INFO
#include <FishinoDebug.h>
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Pin dei dati
#define PIN 6
// RTC
RTC_DS1307 rtc;
// Striscia di led
Adafruit_NeoPixel strip = Adafruit_NeoPixel(195, PIN, NEO_GRB + NEO_KHZ800);

// Matrice che identifica i led
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
    {194, 169, 168, 143, 142, 117, 116, 91, 90, 65, 64, 39, 38, 13, 12}};

// Colori principali dei led
const uint32_t red = strip.Color(255, 0, 0);
const uint32_t green = strip.Color(0, 255, 0);
const uint32_t blue = strip.Color(0, 0, 255);
const uint32_t white = strip.Color(255, 255, 255);
const uint32_t black = strip.Color(0, 0, 0);
const uint32_t orange = strip.Color(255, 165, 0);

// Valori dell'orario
int hour = 0;
int minute = 0;
int second = 0;

// Se si deve usare la parola "meno" cambiano le parole sul word clock
bool meno = false;

// Definizione network
#ifndef __MY_NETWORK_H
// SSID della rete WiFi
#define MY_SSID "Potaspot"
// Password della rete WiFi (Usare "" per rete non protetta)
#define MY_PASS "MeterPeterPota"
// Commentare la linea seguente per usare DHCP
//#define IPADDR    192, 168,   1, 251
#define GATEWAY 192, 168, 1, 1
#define NETMASK 255, 255, 255, 0
#endif

// Delay della richiesta UDP
unsigned long delayUDP;
// Delay del refresh dei pixel
unsigned long delayPixels;

// Impostazione network
#ifdef IPADDR
IPAddress ip(IPADDR);
#ifdef GATEWAY
IPAddress gw(GATEWAY);
#else
IPAddress gw(ip[0], ip[1], ip[2], 1);
#endif
#ifdef NETMASK
IPAddress nm(NETMASK);
#else
IPAddress nm(255, 255, 255, 0);
#endif
#endif

// Porta che ascolta i pacchetti UDP
unsigned const int localPort = 2390;
// Indirizzo IP time server - time.nist.gov
IPAddress timeServer(129, 6, 15, 28);
// Informazioni sull'orario NTP sono nei primi 48 bytes del messaggio
const int NTP_PACKET_SIZE = 48;
// Buffer che individua i pacchetti
byte packetBuffer[NTP_PACKET_SIZE];
// Istanza UPD che gestisce l'invio e la ricezione di pacchetti
FishinoUDP Udp;

/*
Setup del sistema
*/
void setup()
{
  // Delay all'orario corrente per fargli fare subito la prima richiesta
  delayUDP = millis();
  // Delay dei led a 1 secondo
  delayPixels = millis() + 1000;
  // Inizializza la porta seriale e ne attende l'apertura
  Serial.begin(115200);
  // Riavvia e testa il modulo WiFi
  while (!Fishino.reset())
  {
    Serial << F("Fishino RESET FAILED, RETRYING...\n");
  }
  Serial << F("Fishino WiFi RESET OK\n");
  // Imposta la modalità stazione (default per una connessione WiFi)
  Fishino.setMode(STATION_MODE);
  // Tenta la connessione finchè non riesce
  Serial << F("Connessione al AP...");
  while (!Fishino.begin(MY_SSID, MY_PASS))
  {
    Serial << ".";
    delay(2000);
  }
  Serial << "OK\n";
// Imposta l'IP statico oppure avvia il client DHCP
#ifdef IPADDR
  Fishino.config(ip, gw, nm);
#else
  Fishino.staStartDHCP();
#endif

  // Aspetta finché non c'è una connessione stabile
  Serial << F("Aspettando un IP...");
  while (Fishino.status() != STATION_GOT_IP)
  {
    Serial << ".";
    delay(500);
  }
  Serial << "OK\n";
  // Stampa lo stato della connessione sulla porta seriale
  printWifiStatus();
  // Inizia la connessione con il server e ascolta i pacchetti
  Serial << F("Inizio connessione al server...\n");
  Udp.begin(localPort);
  // Inizializzazione della striscia di led
  strip.begin();
  strip.setBrightness(255);
  strip.show();
  // Impostazione del RTC
  if (!rtc.begin())
  {
    Serial.println("Impossibile trovare RTC");
    while (1)
      ;
  }
  // Verifica funzionamento RTC
  // Inserisce l'orario del computer durante la compilazione
  else if (!rtc.isrunning())
  {
    Serial.println("RTC non è in funzione!");
  }
  else
  {
    // Se vuoi un orario personalizzato, togli il commento alla riga successiva
    // l'orario: ANNO, MESE, GIORNI, ORA, MINUTI, SECONDI
    //rtc.adjust(DateTime(2014, 1, 12, 0, 59, 40));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

/*
Esecuzione del sistema
*/
void loop()
{
  // Controllo la richiesta iniziale
  if (millis() > delayUDP)
  {
    // Invio richiesta pacchetto
    sendRequest(1000);
    while (Udp.parsePacket())
    {
      // Stampo il tempo iniziale
      unsigned long secsSince1900 = getPacket();
      setInitialTime(secsSince1900);
    }
    // Attende prima di effettuare una nuova richiesta
    delayUDP = millis() + 300000;
  }
  else
  {
    // Stampo l'orario sul word clock
    setWordClock();
  }
}

/*
Stampa lo stato della connessione WiFi
*/
void printWifiStatus()
{
  // Stampa lo SSID della rete
  Serial.print("SSID: ");
  Serial.println(Fishino.SSID());
  // Stampa l'indirizzo IP della rete
  IPAddress ip = Fishino.localIP();
  Serial << F("IP Address: ");
  Serial.println(ip);
  // Stampa la potenza del segnale di rete
  long rssi = Fishino.RSSI();
  Serial << F("Potenza segnale (RSSI):");
  Serial.print(rssi);
  Serial << F(" dBm\n");
}

/*
Invia una richiesta al server NTP all'indirizzo fornito
*/
void sendNTPpacket(IPAddress &address)
{
  // Azzera il buffer di ricezione NTP
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Inizializza i valori da inviare al server NTP
  // (vedere URL del server per dettagli sul formato pacchetto)
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // Tutti i campi del paccketto NTP sono stati impostati
  // è quindi possibile inviare il pacchetto di richiesta di data/ora

  // invia la richiesta NTP alla porta 123
  // beginPacket() apre solo la connessione
  Udp.beginPacket(address, 123);
  // Riempie il buffer di invio UDP con i dati del pacchetto
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  // Termina e invia il pacchetto
  Udp.endPacket();
}

/*
Invio di una richiesta al server
*/
void sendRequest(unsigned long waitTime)
{
  // Invio pacchetto al server NTP
  Serial << F("Invio richiesta UDP...");
  sendNTPpacket(timeServer);
  Serial << "OK\n";
  // Attesa per vedere se una risposta è disponibile
  delay(waitTime);
}

/*
Metodo che imposta il pacchetto e ne ricava i secondi passati al 01/01/1900
*/
unsigned long getPacket()
{
  Serial << F("Pacchetto ricevuto\n");
  // Stampa IP e porta remoti per mostrare la provenienza del pacchetto
  IPAddress remoteIp = Udp.remoteIP();
  uint32_t remotePort = Udp.remotePort();
  Serial << F("Remote IP   : ") << remoteIp << "\n";
  Serial << F("Remote port : ") << remotePort << "\n";
  // Abbiamo ricevuto un pacchetto, leggiamo i dati e inseriamoli in un buffer
  Udp.read(packetBuffer, NTP_PACKET_SIZE);
  // Il timestamp inizia dal byte 40 del pacchetto ricevuto, e consiste in 4 bytes
  // o due words, long. Innanzitutto estraiamo le due words
  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  // Combiniamo i 4 bytes (o 2 words) in un long integer
  // che è il tempo NTP (secondi dal primo Gennaio 1900)
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  Serial << F("Seconds since Jan 1 1900 = ") << secsSince1900 << "\n";
  return secsSince1900;
}

/*
Lettura iniziale del'orario trasformato in Unix Time
*/
void setInitialTime(unsigned long secsSince1900)
{
  // Ora convertiamo il tempo NTP in formato leggibile
  Serial << F("Unix time = ");
  // Il tempo Unix inizia dal 01/01/1970. In secondi, sono 2208988800
  // Sottrae dal tempo NTP la base Unix
  unsigned long epoch = secsSince1900 - 2208988800UL;
  // Stampa il tempo Unix
  Serial << epoch << "\n";
  // Tempo UTC (ora al meridiano di Greenwich, GMT)
  Serial << F("The ETC time is ");
  // Stampa l'ora (contando 86400 secondi al giorno
  Serial << ((epoch % 86400L) / 3600 + 1);
  Serial.print(':');
  if (((epoch % 3600) / 60) < 10)
  {
    // Nei primi 10 minuti di ogni ora vogiamo uno zero iniziale
    Serial << '0';
  }
  // Stampa i minuti (contando 3600 secondi per minuto)
  Serial << ((epoch % 3600) / 60);
  Serial << ':';
  if ((epoch % 60) < 10)
  {
    // Nei primi 10 secondi di ogni minuto vogliamo lo zero iniziale
    Serial << '0';
  }
  // Stampa i secondi
  Serial << epoch % 60 << "\n";
}

/*
Stampo l'orario sul word clock
*/
void setWordClock()
{
  // Istanza data e ora
  DateTime now = rtc.now();
  hour = now.hour();
  minute = now.minute();
  second = now.second();
  // Stampo le informazioni in decimale
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(") ");
  Serial.println();
  // Stampo l'orario sul word clock
  printTime(hour, minute, second);
  delayPixels = millis() + 1000;
}

/*
Metodo che accende i led del word clock in base all'orario passato
*/
void printTime(int hour, int minute, int second)
{
  meno = false;
  // Stampo "pausa"
  printBreak(hour, minute);

  // Stampo l'ora
  printHour(hour, minute);

  // E
  if (minute >= 5 && minute <= 35)
  {
    e(white);
  }
  else
  {
    e(black);
  }

  // Illuminazione dei pallini
  int diff = minute - int(minute / 10) * 10;

  // Minuti
  if (minute < 35)
  {
    if (minute % 5 != 0)
    {
      generateWord(0, 1, 1, white);
      generateWord(0, 2, 2, black);
    }
    else
    {
      generateWord(0, 1, 6, black);
    }

    if (diff != 0 && diff != 5)
    {
      if (diff >= 1 && diff <= 4 || diff >= 6 && diff <= 9)
      {
        generateWord(0, 3, 3, white);
        generateWord(0, 4, 6, black);
      }
      if (diff >= 2 && diff <= 4 || diff >= 7 && diff <= 9)
      {
        generateWord(0, 3, 4, white);
        generateWord(0, 5, 6, black);
      }
      if (diff >= 3 && diff <= 4 || diff >= 8 && diff <= 9)
      {
        generateWord(0, 3, 5, white);
        generateWord(0, 6, 6, black);
      }
      if (diff == 4 || diff == 9)
      {
        generateWord(0, 3, 6, white);
      }
    }

    if (minute >= 5 && minute < 10)
    {
      cinque(white);
    }
    else if (minute >= 10 && minute < 15)
    {
      cinque(black);
      dieci(white);
    }
    else if (minute >= 15 && minute < 20)
    {
      dieci(black);
      unQuarto(white);
    }
    else if (minute >= 20 && minute < 25)
    {
      unQuarto(black);
      venti(white);
    }
    else if (minute >= 25 && minute < 30)
    {
      venti(black);
      venticinque(white);
    }
    else if (minute >= 30 && minute < 35)
    {
      venticinque(black);
      eMezza(white);
    }
  }
  else
  {

    if (minute < 55)
    {
      if (minute != 35)
      {
        // Meno
        meno(white);
        meno = true;
      }
    }

    if (minute % 5 != 0)
    {
      if (minute < 55)
      {
        generateWord(0, 1, 1, white);
      }
    }
    else
    {
      generateWord(0, 1, 6, black);
    }

    if (diff != 0 && diff != 5)
    {

      if (diff == 2 || diff == 7)
      {
        generateWord(0, 3, 5, white);
        generateWord(0, 6, 6, black);
        if (minute > 55)
        {
          generateWord(9, 1, 4, black);
        }
      }
      else if (diff == 3 || diff == 8)
      {
        generateWord(0, 3, 4, white);
        generateWord(0, 5, 6, black);
        if (minute > 55)
        {
          generateWord(9, 1, 4, black);
        }
      }
      else if (diff == 4 || diff == 9)
      {
        generateWord(0, 3, 3, white);
        generateWord(0, 4, 6, black);
        if (minute > 55)
        {
          generateWord(9, 1, 4, black);
        }
      }
      else if (diff == 1 || diff == 6)
      {
        generateWord(0, 3, 6, white);
        if (minute > 55)
        {
          generateWord(9, 1, 4, black);
        }
      }
    }
    if (minute > 55)
    {
      generateWord(0, 2, 2, white);
      generateWord(0, 1, 1, black);
      generateWord(12, 9, 14, black);
      generateWord(9, 1, 4, black);
      //generateWord(12, 9, 14, white);
    }
    else if (minute > 50)
    {
      generateWord(12, 9, 14, white);
      generateWord(10, 1, 5, black);
    }
    else if (minute > 45)
    {
      generateWord(10, 1, 5, white);
      generateWord(10, 6, 7, black);
      generateWord(10, 9, 14, black);
    }
    else if (minute > 40)
    {
      generateWord(10, 6, 7, white);
      generateWord(10, 9, 14, white);
      generateWord(11, 1, 5, black);
    }
    else if (minute >= 35)
    {
      if (minute != 35)
      {
        generateWord(11, 1, 5, white);
      }
      generateWord(12, 4, 14, black);
      generateWord(7, 1, 5, black);
    }
  }

  // Stampo i secondi sul word clock
  printSecond(second);
}

/*
Stampa della parola "pausa" sul word clock
*/
void printBreak(int hour, int minute)
{
  uint32_t color = black;
  // Pausa OK (9:50 - 10:01 / 14:45 - 14:56)
  if (
      (hour == 9 && minute >= 50) ||
      (hour == 10 && minute >= 0 && minute < 2) ||
      (hour == 14 && minute >= 45 && minute < 57))
  {
    color = green;
  }
  // Pausa sta per finire (10:02 - 10:05 / 14:57 - 15:00)
  else if (
      (hour == 10 && minute >= 2 && minute < 5) ||
      (hour == 14 && minute >= 57 && minute <= 59))
  {
    color = red;
  }
  // Pausa non c'è, led spenti
  else
  {
    color = black;
  }
  // Imposto la parola
  pausa(color);
}

/*
Accendo "pausa" del colore passato
*/
void pausa(uint32_t color)
{
  generateWord(0, 8, 12, color);
}

/*
Accendo "è" del colore passato
*/
void egrave(uint32_t color)
{
  generateWord(1, 1, 1, color);
}

/*
Accendo "sono le" del colore passato
*/
void sonoLe(uint32_t color)
{
  generateWord(1, 2, 5, color);
  generateWord(1, 7, 8, color);
}

/*
Accendo "l'una" del colore passato
*/
void una(uint32_t color)
{
  generateWord(1, 10, 14, color);
}

/*
Accendo "mezzogiorno" del colore passato
*/
void mezzogiorno(uint32_t color)
{
  generateWord(2, 1, 11, color);
}

/*
Accendo "due" del colore passato
*/
void due(uint32_t color)
{
  generateWord(2, 12, 14, color);
}

/*
Accendo "tre" del colore passato
*/
void tre(uint32_t color)
{
  generateWord(3, 1, 3, color);
}

/*
Accendo "quattro" del colore passato
*/
void quattro(uint32_t color)
{
  generateWord(3, 5, 11, color);
}

/*
Accendo "cinque" del colore passato
*/
void cinque(uint32_t color)
{
  // Decido se accendere il "cinque" dopo il meno o viceversa
  if (meno)
  {
    generateWord(9, 9, 14, color);
    generateWord(4, 1, 6, black);
  }
  else
  {
    generateWord(9, 9, 14, black);
    generateWord(4, 1, 6, color);
  }
}

/*
Accendo "sei" del colore passato
*/
void sei(uint32_t color)
{
  generateWord(3, 12, 14, color);
}

/*
Accendo "sette" del colore passato
*/
void sette(uint32_t color)
{
  generateWord(4, 10, 14, color);
}

/*
Accendo "otto" del colore passato
*/
void otto(uint32_t color)
{
  generateWord(5, 1, 4, color);
}

/*
Accendo "nove" del colore passato
*/
void nove(uint32_t color)
{
  generateWord(5, 5, 8, color);
}

/*
Accendo "dieci" del colore passato
*/
void dieci(uint32_t color)
{
  // Decido se accendere il "dieci" dopo il meno o viceversa
  if (meno)
  {
    generateWord(5, 10, 14, black);
    generateWord(10, 1, 5, color);
  }
  else
  {
    generateWord(5, 10, 14, color);
    generateWord(10, 1, 5, black);
  }
}

/*
Accendo "undici" del colore passato
*/
void undici(uint32_t color)
{
  generateWord(6, 1, 6, color);
}

/*
Accendo "undici" del colore passato
*/
void undici(uint32_t color)
{
  generateWord(6, 1, 6, color);
}

/*
Accendo "e" del colore passato
*/
void e(uint32_t color)
{
  generateWord(6, 10, 10, color);
}

/*
Accendo "mezzanotte" del colore passato
*/
void mezzanotte(uint32_t color)
{
  generateWord(7, 1, 10, color);
}

/*
Accendo "e trentacinque" del colore passato
*/
void eTrentacinque(uint32_t color)
{
  e(color);
  generateWord(8, 1, 12, color);
}

/*
Accendo "meno" del colore passato
*/
void meno(uint32_t color)
{
  generateWord(9, 1, 4, color);
}

/*
Accendo "un quarto" del colore passato
*/
void unQuarto(uint32_t color)
{
  generateWord(10, 6, 7, color);
  generateWord(10, 9, 14, color);
}

/*
Accendo "venti" del colore passato
*/
void venti(uint32_t color)
{
  generateWord(11, 1, 5, color);
}

/*
Accendo "e mezza" del colore passato
*/
void eMezza(uint32_t color)
{
  generateWord(11, 1, 5, color);
  generateWord(11, 1, 5, color);
}

/*
Accendo "venticinque" del colore passato
*/
void venticinque(uint32_t color)
{
  generateWord(12, 4, 14, color);
}

/*
Spengo tutti i led del word clock
*/
void turnAllOff()
{
  for (int i = 0; i < strip.numPixels(); i++)
  {
    pixelOn(i, black);
  }
}

/*
Accensione dell'ora
*/
printHour(int hour, int minute)
{
  switch (hour)
  {
  case 0:
    egrave(white);
    mezzanotte(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      // L'una
      turnAllOff();
      egrave(white);
      una(white);
    }
    break;

  case 12:
    egrave(white);
    mezzogiorno(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      // L'una
      turnAllOff();
      egrave(white);
      una(white);
    }
    break;

  case 1:
  case 13:
    egrave(white);
    una(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      // Due
      turnAllOff();
      sonoLe(white);
      due(white);
    }
    break;

  case 2:
  case 14:
    sonoLe(white);
    due(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      // Tre
      turnAllOff();
      sonoLe(white);
      tre(white);
    }
    break;

  case 3:
  case 15:
    sonoLe(white);
    tre(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      // Quattro
      turnAllOff();
      sonoLe(white);
      quattro(white);
    }
    break;

  case 4:
  case 16:
    sonoLe(white);
    quattro(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      // Cinque
      turnAllOff();
      sonoLe(white);
      cinque(white);
    }
    break;

  case 5:
  case 17:
    sonoLe(white);
    cinque(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      turnAllOff();
      sonoLe(white);
      sei(white);
    }
    break;

  case 6:
  case 18:
    sonoLe(white);
    sei(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      turnAllOff();
      sonoLe(white);
      sette(white);
    }
    break;

  case 7:
  case 19:
    sonoLe(white);
    sette(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      turnAllOff();
      sonoLe(white);
      otto(white);
    }
    break;

  case 8:
  case 20:
    sonoLe(white);
    otto(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      turnAllOff();
      sonoLe(white);
      nove(white);
    }
    break;

  case 9:
  case 21:
    sonoLe(white);
    nove(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      turnAllOff();
      sonoLe(white);
      dieci(white);
    }
    break;

  case 10:
  case 22:
    sonoLe(white);
    dieci(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      turnAllOff();
      sonoLe(white);
      undici(white);
    }
    break;

  case 11:
  case 23:
    sonoLe(white);
    undici(white);
    if (minute == 35)
    {
      eTrentacinque(white);
    }
    else if (minute > 35)
    {
      turnAllOff();
      egrave(white);
      if (hour == 11)
      {
        mezzogiorno(white);
      }
      else if (hour == 23)
      {
        mezzanotte(white);
      }
      break;

    default:
      turnAllOff();
      break;
    }
  }
}

/*
Generazione dei secondi
*/
void generateSeconds(int length, uint32_t color)
{
  for (int i = 1; i <= length; i++)
  {
    generateWord(i, 0, 0, color);
  }
}

/*
Accensione dei secondi
*/
void printSecond(int second)
{
  switch (second)
  {
  case 0 ... 4:
    generateSeconds(12, black);
    generateSeconds(1, white);
    break;

  case 5 ... 9:
    generateSeconds(2, white);
    break;

  case 10 ... 14:
    generateSeconds(3, white);
    break;

  case 15 ... 19:
    generateSeconds(4, white);
    break;

  case 20 ... 24:
    generateSeconds(5, white);
    break;

  case 25 ... 29:
    generateSeconds(6, white);
    break;

  case 30 ... 34:
    generateSeconds(7, white);
    break;

  case 35 ... 39:
    generateSeconds(8, white);
    break;

  case 40 ... 44:
    generateSeconds(9, white);
    break;

  case 45 ... 49:
    generateSeconds(10, white);
    break;

  case 50 ... 54:
    generateSeconds(11, white);
    break;

  case 55 ... 59:
    generateSeconds(12, white);
    break;
  }
}

/*
Accensione di un led di un certo colore
*/
void pixelOn(int pixel, uint32_t color)
{
  strip.setPixelColor(pixel, color);
  strip.show();
}

/*
Generazione di una parola nel word clock
Si fa riferimento alla riga, l'indice d'inizio e quello di fine
*/
void generateWord(int row, int start, int end, uint32_t color)
{
  if (start == 0 && end == 0)
  {
    for (int i = start; i <= (end - start); i++)
    {
      pixelOn(pixels[row][i], color);
    }
  }
  else
  {
    for (int i = start; i <= end; i++)
    {
      pixelOn(pixels[row][i], color);
    }
  }
}