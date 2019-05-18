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
    Serial << F("Fishino RESET FAILED, RETRYING...\n");
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
  if (!rtc.isrunning())
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

void printTime(int hour, int minute, int second)
{
  int diff;
  boolean meno = false;

  int pausaHour = hour;

  //PAUSA
  //Mattino
  Serial.println(hour);
  Serial.println(minute);
  if ((pausaHour == 9 && minute >= 50) || (pausaHour == 10 && minute >= 0 && minute < 2))
  {
    generateWord(0, 8, 12, green);
  }
  else if (pausaHour == 10 && minute >= 2 && minute <= 4)
  {
    generateWord(0, 8, 12, red);
  }
  //Pomeriggio
  else if (pausaHour == 14 && minute >= 45 && minute <= 56)
  {
    generateWord(0, 8, 12, green);
  }
  else if (pausaHour == 14 && minute >= 57 && minute <= 59)
  {
    generateWord(0, 8, 12, red);
  }
  //Non pausa
  else
  {
    generateWord(0, 8, 12, black);
  }

  //Più o meno
  if (minute < 35)
  {
    meno = false;
  }
  else
  {
    meno = true;
  }

  if (hour == 12)
  {
    if (minute < 35)
    {
      generateWord(1, 1, 1, white);
      generateWord(2, 1, 11, white);
    }
    else if (minute == 35)
    {
      generateWord(1, 1, 1, white);
      generateWord(2, 1, 11, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 1, 1, white);
      generateWord(2, 1, 11, black);
      generateWord(1, 10, 14, white);
    }
    //time += "mezzogiorno ";
  }
  else if (hour == 0)
  {
    if (minute < 35)
    {
      generateWord(1, 1, 1, white);
      generateWord(7, 1, 10, white);
    }
    else if (minute == 35)
    {
      generateWord(1, 1, 1, white);
      generateWord(7, 1, 10, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(7, 1, 10, black);
      generateWord(1, 10, 14, white);
      generateWord(1, 1, 1, white);
    }
    //time += "mezzanotte ";
  }
  else if (hour == 1 || hour == 13)
  {
    if (minute < 35)
    {
      generateWord(1, 1, 1, white);
      generateWord(1, 10, 14, white);
    }
    else if (minute == 35)
    {
      generateWord(1, 1, 1, white);
      generateWord(1, 10, 14, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(1, 1, 1, black);
      generateWord(1, 10, 14, black);
      generateWord(2, 12, 14, white);
    }
  }
  else if (hour == 2 || hour == 14)
  {
    if (minute < 35)
    {
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(2, 12, 14, white);
    }
    else if (minute == 35)
    {
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(2, 12, 14, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(2, 12, 14, black);
      generateWord(3, 1, 3, white);
    }
    //time += "due ";
  }
  else if (hour == 3 || hour == 15)
  {
    //tre();
    if (minute < 35)
    {
      generateWord(3, 1, 3, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
    }
    else if (minute == 35)
    {
      generateWord(3, 1, 3, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(3, 1, 3, black);
      generateWord(3, 5, 11, white);
    }
    //time += "tre ";
  }
  else if (hour == 4 || hour == 16)
  {
    generateWord(3, 5, 11, white);
    generateWord(1, 2, 5, white);
    generateWord(1, 7, 8, white);
    if (minute < 35)
    {
      generateWord(3, 5, 11, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
    }
    else if (minute == 35)
    {
      generateWord(3, 5, 11, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(3, 5, 11, black);
      generateWord(4, 1, 6, white);
    }
    //time += "quattro ";
  }
  else if (hour == 5 || hour == 17)
  {
    if (minute < 35)
    {
      generateWord(4, 1, 6, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
    }
    else if (minute == 35)
    {
      generateWord(4, 1, 6, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(4, 1, 6, black);
      generateWord(3, 12, 14, white);
    }
    //time += "cinque ";
  }
  else if (hour == 6 || hour == 18)
  {
    if (minute < 35)
    {
      generateWord(3, 12, 14, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
    }
    else if (minute == 35)
    {
      generateWord(3, 12, 14, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(3, 12, 14, black);
      generateWord(4, 10, 14, white);
    }
    //time += "sei ";
  }
  else if (hour == 7 || hour == 19)
  {
    if (minute < 35)
    {
      generateWord(4, 10, 14, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
    }
    else if (minute == 35)
    {
      generateWord(4, 10, 14, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(4, 10, 14, black);
      generateWord(5, 1, 4, white);
    }
    //time += "sette ";
  }
  else if (hour == 8 || hour == 20)
  {
    if (minute < 35)
    {
      generateWord(5, 1, 4, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
    }
    else if (minute == 35)
    {
      generateWord(5, 1, 4, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(5, 1, 4, black);
      generateWord(5, 5, 8, white);
    }
    //time += "otto ";
  }
  else if (hour == 9 || hour == 21)
  {
    if (minute < 35)
    {
      generateWord(5, 5, 8, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
    }
    else if (minute == 35)
    {
      generateWord(5, 5, 8, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(5, 5, 8, black);
      generateWord(5, 10, 14, white);
    }
    //time += "nove ";
  }
  else if (hour == 10 || hour == 22)
  {
    if (minute < 35)
    {
      generateWord(5, 10, 14, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
    }
    else if (minute == 35)
    {
      generateWord(5, 10, 14, white);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(8, 1, 12, white);
    }
    if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(5, 10, 14, black);
      generateWord(6, 1, 6, white);
    }
    //time += "dieci ";
  }
  else if (hour == 11 || hour == 23)
  {
    if (minute < 35)
    {
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(6, 1, 6, white);
    }
    else if (minute == 35)
    {
      generateWord(1, 2, 5, white);
      generateWord(1, 7, 8, white);
      generateWord(6, 1, 6, white);
      generateWord(8, 1, 12, white);
    }
    else if (minute > 35)
    {
      generateWord(8, 1, 12, black);
      generateWord(1, 2, 5, black);
      generateWord(1, 7, 8, black);
      generateWord(6, 1, 6, black);
      generateWord(1, 1, 1, white);
      if (hour == 11)
      {
        generateWord(2, 1, 11, white);
      }
      else if (hour == 23)
      {
        generateWord(7, 1, 10, white);
      }
    }
    //time += "undici ";
  }

  //e
  if (minute >= 5 && minute <= 35)
  {
    generateWord(6, 10, 10, white);
  }
  else
  {
    generateWord(6, 10, 10, black);
  }

  //Illuminazione dei pallini
  diff = minute - int(minute / 10) * 10;

  //Minuti
  if (meno == false)
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
      //genWord(90, 5, 10, on); //Five on
      generateWord(8, 7, 12, white);
    }
    else if (minute >= 10 && minute < 15)
    {
      //genWord(100, 0, 4, on);
      generateWord(10, 1, 5, white);
      generateWord(8, 7, 12, black);
    }
    else if (minute >= 15 && minute < 20)
    {
      //genWord(80, 2, 3, on);
      //genWord(80, 5, 10, on);
      generateWord(10, 6, 7, white);
      generateWord(10, 9, 14, white);
      generateWord(10, 1, 5, black);
    }
    else if (minute >= 20 && minute < 25)
    {
      //genWord(90, 0, 4, on); //Twenty on
      generateWord(10, 6, 7, black);
      generateWord(10, 9, 14, black);
      generateWord(11, 1, 5, white);
    }
    else if (minute >= 25 && minute < 30)
    {
      //genWord(90, 0, 10, on); //25 on
      generateWord(12, 4, 14, white);
      generateWord(11, 1, 5, black);
    }
    else if (minute >= 30 && minute < 35)
    {
      //genWord(100, 6, 10, on); //Half on
      generateWord(7, 1, 5, white);
      generateWord(12, 4, 14, black);
    }
  }
  else
  {

    if (minute < 55)
    {
      if (minute != 35)
      {
        generateWord(9, 1, 4, white); //Meno
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

  //secondi
  if (second >= 0 && second <= 4)
  {
    for (int i = 0; i < 13; i++)
    {
      generateWord(i, 0, 0, black);
    }
    generateWord(1, 0, 0, white);
  }
  else if (second >= 5 && second <= 9)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
  }
  else if (second >= 10 && second <= 14)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
    generateWord(3, 0, 0, white);
  }
  else if (second >= 15 && second <= 19)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
    generateWord(3, 0, 0, white);
    generateWord(4, 0, 0, white);
  }
  else if (second >= 20 && second <= 24)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
    generateWord(3, 0, 0, white);
    generateWord(4, 0, 0, white);
    generateWord(5, 0, 0, white);
  }
  else if (second >= 25 && second <= 29)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
    generateWord(3, 0, 0, white);
    generateWord(4, 0, 0, white);
    generateWord(5, 0, 0, white);
    generateWord(6, 0, 0, white);
  }
  else if (second >= 30 && second <= 34)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
    generateWord(3, 0, 0, white);
    generateWord(4, 0, 0, white);
    generateWord(5, 0, 0, white);
    generateWord(6, 0, 0, white);
    generateWord(7, 0, 0, white);
  }
  else if (second >= 35 && second <= 39)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
    generateWord(3, 0, 0, white);
    generateWord(4, 0, 0, white);
    generateWord(5, 0, 0, white);
    generateWord(6, 0, 0, white);
    generateWord(7, 0, 0, white);
    generateWord(8, 0, 0, white);
  }
  else if (second >= 40 && second <= 44)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
    generateWord(3, 0, 0, white);
    generateWord(4, 0, 0, white);
    generateWord(5, 0, 0, white);
    generateWord(6, 0, 0, white);
    generateWord(7, 0, 0, white);
    generateWord(8, 0, 0, white);
    generateWord(9, 0, 0, white);
  }
  else if (second >= 45 && second <= 49)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
    generateWord(3, 0, 0, white);
    generateWord(4, 0, 0, white);
    generateWord(5, 0, 0, white);
    generateWord(6, 0, 0, white);
    generateWord(7, 0, 0, white);
    generateWord(8, 0, 0, white);
    generateWord(9, 0, 0, white);
    generateWord(10, 0, 0, white);
  }
  else if (second >= 50 && second <= 54)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
    generateWord(3, 0, 0, white);
    generateWord(4, 0, 0, white);
    generateWord(5, 0, 0, white);
    generateWord(6, 0, 0, white);
    generateWord(7, 0, 0, white);
    generateWord(8, 0, 0, white);
    generateWord(9, 0, 0, white);
    generateWord(10, 0, 0, white);
    generateWord(11, 0, 0, white);
  }
  else if (second >= 55 && second <= 59)
  {
    generateWord(1, 0, 0, white);
    generateWord(2, 0, 0, white);
    generateWord(3, 0, 0, white);
    generateWord(4, 0, 0, white);
    generateWord(5, 0, 0, white);
    generateWord(6, 0, 0, white);
    generateWord(7, 0, 0, white);
    generateWord(8, 0, 0, white);
    generateWord(9, 0, 0, white);
    generateWord(10, 0, 0, white);
    generateWord(11, 0, 0, white);
    generateWord(12, 0, 0, white);
  }
}
void pixelOn(int pixel, uint32_t color)
{
  strip.setPixelColor(pixel, color);
  strip.show();
}
void generateWord(int row, int min, int max, uint32_t color)
{
  if (min == 0 && max == 0)
  {
    for (int i = min; i <= (max - min); i++)
    {
      pixelOn(pixels[row][i], color);
    }
  }
  else
  {
    for (int i = min; i <= max; i++)
    {
      pixelOn(pixels[row][i], color);
    }
  }
}
