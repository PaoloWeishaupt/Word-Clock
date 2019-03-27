/*
	Udp NTP Client

	Get the time from a Network Time Protocol (NTP) time server
	Demonstrates use of UDP sendPacket and ReceivePacket
	For more on NTP time servers and the messages needed to communicate with them,
	see http://en.wikipedia.org/wiki/Network_Time_Protocol

	created 4 Sep 2010
	by Michael Margolis
	modified 9 Apr 2012
	by Tom Igoe

	This code is in the public domain.

	2016_02_10 Adapted to Fishino by Massimo Del Fedele

 */
#include <SPI.h>
#include <Fishino.h>

#define DEBUG_LEVEL_INFO
#include <FishinoDebug.h>
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// CONFIGURAZIONE NEOPIXEL
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
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION DATA		-- ADAPT TO YOUR NETWORK !!!
// DATI DI CONFIGURAZIONE	-- ADATTARE ALLA PROPRIA RETE WiFi !!!

#ifndef __MY_NETWORK_H

// here pur SSID of your network
// inserire qui lo SSID della rete WiFi
#define MY_SSID	"Potaspot"

// here put PASSWORD of your network. Use "" if none
// inserire qui la PASSWORD della rete WiFi -- Usare "" se la rete non ￨ protetta
#define MY_PASS	"MeterPeterPota"

// comment this line if you want a dynamic IP through DHCP
// obtained IP will be printed on serial port monitor
// commentare la linea seguente per avere un IP dinamico tramite DHCP
// l'IP ottenuto verrà visualizzato sul monitor seriale
//#define IPADDR		192, 168,   1, 251
#define GATEWAY		192, 168,   1,   1
#define NETMASK		255, 255, 255,   0

#endif
//                    END OF CONFIGURATION DATA                      //
//                       FINE CONFIGURAZIONE                         //
///////////////////////////////////////////////////////////////////////

// define ip address if required
// NOTE : if your network is not of type 255.255.255.0 or your gateway is not xx.xx.xx.1
// you should set also both netmask and gateway
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

// local port to listen for UDP packets
unsigned int localPort = 2390;

// time.nist.gov NTP Server
IPAddress timeServer(129, 6, 15, 28);

// NTP time stamp is in the first 48 bytes of the message
const int NTP_PACKET_SIZE = 48;

//buffer to hold incoming and outgoing packets
byte packetBuffer[NTP_PACKET_SIZE];

// A UDP instance to let us send and receive packets over UDP
FishinoUDP Udp;

void printWifiStatus()
{
	// print the SSID of the network you're attached to:
	// stampa lo SSID della rete:
	Serial.print("SSID: ");
	Serial.println(Fishino.SSID());

	// print your WiFi shield's IP address:
	// stampa l'indirizzo IP della rete:
	IPAddress ip = Fishino.localIP();
	Serial << F("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	// stampa la potenza del segnale di rete:
	long rssi = Fishino.RSSI();
	Serial << F("signal strength (RSSI):");
	Serial.print(rssi);
	Serial << F(" dBm\n");
}

// send an NTP request to the time server at the given address
// invia una richiesta al server NTP all'indirizzo fornito
unsigned long sendNTPpacket(IPAddress& address)
{
	// set all bytes in the buffer to 0
	// azzera il buffer di ricezione NTP
	memset(packetBuffer, 0, NTP_PACKET_SIZE);

	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	// Inizializza i valori da inviare al server NTP
	// (vedere URL del server per dettagli sul formato pacchetto)

	// LI, Version, Mode
	packetBuffer[0] = 0b11100011;

	// Stratum, or type of clock
	packetBuffer[1] = 0;

	// Polling Interval
	packetBuffer[2] = 6;

	// Peer Clock Precision
	packetBuffer[3] = 0xEC;

	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	// tutti i campi del paccketto NTP sono stati impostati
	// è quindi possibile inviare il paccetto di richiesta di data/ora

	// NTP requests are to port 123
	// beginPacket() just opens the connection
	// invia la richiesta NTP alla porta 123
	// beginPacket() apre solo la connessione
	Udp.beginPacket(address, 123);

	// fill UDP buffer with packet data
	// riempie il buffer di invio UDP con i dati del pacchetto
	Udp.write(packetBuffer, NTP_PACKET_SIZE);

	// ends and send the UDP packet
	// termina ed invia il pacchetto UDP
	Udp.endPacket();

	return 0;
}

void setup()
{
	// Initialize serial and wait for port to open
	// Inizializza la porta seriale e ne attende l'apertura
	Serial.begin(115200);

	// only for Leonardo needed
	// necessario solo per la Leonardo
	while (!Serial)
		;

	// reset and test WiFi module
	// resetta e testa il modulo WiFi
	while(!Fishino.reset())
		Serial << F("Fishino RESET FAILED, RETRYING...\n");
	Serial << F("Fishino WiFi RESET OK\n");

	// go into station mode
	// imposta la modalità stazione
	Fishino.setMode(STATION_MODE);

	// try forever to connect to AP
	// tenta la connessione finchè non riesce
	Serial << F("Connecting to AP...");
	while(!Fishino.begin(MY_SSID, MY_PASS))
	{
		Serial << ".";
		delay(2000);
	}
	Serial << "OK\n";


	// setup IP or start DHCP client
	// imposta l'IP statico oppure avvia il client DHCP
#ifdef IPADDR
	Fishino.config(ip, gw, nm);
#else
	Fishino.staStartDHCP();
#endif

	// wait till connection is established
	Serial << F("Waiting for IP...");
	while(Fishino.status() != STATION_GOT_IP)
	{
		Serial << ".";
		delay(500);
	}
	Serial << "OK\n";

	// print connection status on serial port
	// stampa lo stato della connessione sulla porta seriale
	printWifiStatus();

	Serial << F("Starting connection to server...\n");
	Udp.begin(localPort);

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
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Se vuoi un orario personalizzato, togli il commento alla riga successiva
    // l'orario: ANNO, MESE, GIORNI, ORA, MINUTI, SECONDI
    //rtc.adjust(DateTime(2014, 1, 12, 18, 0, 0));
  }
}

void loop()
{
	// send an NTP packet to a time server
	Serial << F("Sending UDP request...");
	sendNTPpacket(timeServer);
	Serial << "OK\n";


	// wait to see if a reply is available
	delay(1000);

	while(Udp.parsePacket())
	{
		Serial << F("Packet received\n");

		// print remote port and IP of incoming packet, just to show them
		// stampa IP e porta remoti per mostrare la provenienza del pacchetto
		IPAddress remoteIp = Udp.remoteIP();
		uint32_t remotePort = Udp.remotePort();
		Serial << F("Remote IP   : ") << remoteIp << "\n";
		Serial << F("Remote port : ") << remotePort << "\n";

		// We've received a packet, read the data from it and put into a buffer
		// abbiamo ricevuto un pacchetto, leggiamo i dati ed inseriamoli in un buffer
		Udp.read(packetBuffer, NTP_PACKET_SIZE);

		// the timestamp starts at byte 40 of the received packet and is four bytes,
		// or two words, long. First, extract the two words:
		// il timestamp inizia dal byte 40 del pacchetto ricevuto, e consiste in 4 bytes
		// o due words, long. Innanzitutto estraiamo le due words
		unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
		unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

		// combine the four bytes (two words) into a long integer
		// this is NTP time (seconds since Jan 1 1900):
		// combiniamo i 4 bytes (o 2 words) in un long integer
		// che è il tempo NTP (secondi dal primo Gennaio 1900)
		unsigned long secsSince1900 = highWord << 16 | lowWord;
		Serial << F("Seconds since Jan 1 1900 = ") << secsSince1900 << "\n";

		// now convert NTP time into everyday time
		// ora convertiamo il tempo NTP in formato leggibile
		Serial << F("Unix time = ");

		// Unix time starts on Jan 1 1970. In seconds, that's 2208988800
		// il tempo Unix inizia dal primo Gennaio 1970. In secondi, sono 2208988800
		const unsigned long seventyYears = 2208988800UL;

		// subtract seventy years
		// sottrae dal tempo NTP la base Unix
		unsigned long epoch = secsSince1900 - seventyYears;

		// print Unix time:
		// stampa il tempo Unix
		Serial << epoch << "\n";

		// print the hour, minute and second
		// stampa ora, minuti e secondi

		// UTC is the time at Greenwich Meridian (GMT)
		// Tempo UTC (ora al meridiano di Greenwich, GMT)
		Serial << F("The ETC time is ");

		// print the hour (86400 equals secs per day) + 1 for ETC time
		// stampa l'ora (contando 86400 secondi al giorno
		Serial << ((epoch  % 86400L) / 3600 + 1);
		Serial.print(':');
		if (((epoch % 3600) / 60) < 10)
		{
			// In the first 10 minutes of each hour, we'll want a leading '0'
			// nei primi 10 minuti di ogni ora vogiamo uno zero iniziale
			Serial << '0';
		}

		// print the minute (3600 equals secs per minute)
		// stampa i minuti (contando 3600 secondi per minuto)
		Serial << ((epoch  % 3600) / 60);
		Serial << ':';
		if ((epoch % 60) < 10)
		{
			// In the first 10 seconds of each minute, we'll want a leading '0'
			// nei primi 10 secondi di ogni minuto vogliamo lo zero iniziale
			Serial << '0';
		}
		// print the second
		// stampa i secondi
		Serial << epoch % 60 << "\n";
	}

	// wait ten seconds before asking for the time again
	// attende 10 secondi prima di effettuare una nuova richiesta
	delay(5000);

  DateTime now = rtc.now(); //creo istanza ora/data
  int hour = now.hour();
  int minute = now.minute() + 1;
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
  printTime(hour, minute, second);
  delay(1000);
}
void printTime(int hour, int minute, int second) {
  int diff;
  boolean meno = false;

  //Più o meno
  if(minute < 35){
      meno = false;
  }else{
      meno = true;
      hour += 1;
  }

  if ((hour != 1 && hour != 13) || minute >= 35) {
    generateWord(1, 2, 5, red);
    generateWord(1, 7, 8, red);
  }
  Serial.println(minute);
  if (hour == 1 || hour == 13){
    generateWord(1, 1, 1, red);
    generateWord(1, 10, 14, red);
    //time += "È l'una";
  } else  if (hour == 2 || hour == 14) {
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

  //Illuminazione dei pallini
  diff = minute - int(minute / 10) * 10;
  Serial.println(diff);

  //Minuti
  if(meno == false) {
    if(diff != 0 && diff != 5) {
      //+ on
      //genWord(20, 6, 6, on);

      if (diff != 5 && diff != 0) {
        generateWord(0, 6, 6, red);
      }
      if (diff >= 2 && diff <= 4 || diff >= 7 && diff <= 9) {
        generateWord(0, 8, 8, red);
      }
      if (diff >= 3 && diff <= 4 || diff >= 8 && diff <= 9) {
        generateWord(0, 10, 10, red);
      }
      if (diff == 4 || diff == 9) {
        generateWord(0, 12, 12, red);
      }

      if(minute % 5 != 0){
        generateWord(0, 2, 2, red);
        generateWord(0, 4, 4, black);
      }
      else{
        generateWord(0, 6, 6, black);
        generateWord(0, 8, 8, black);
        generateWord(0, 10, 10, black);
        generateWord(0, 12, 12, black);
        generateWord(0, 4, 4, black);
        generateWord(0, 2, 2, black);
      }
    }
    //e
    //genWord(80, 0, 0, on);
    generateWord(6, 10, 10, red);
    if(minute < 5){
      //genWord(80, 0, 0, off);//Past off
      generateWord(6, 10, 10, black);
    }else if(minute < 10){
      //genWord(90, 5, 10, on); //Five on
      generateWord(8, 7, 12, red);
    }else if (minute < 15){
      //genWord(100, 0, 4, on);
      generateWord(10, 1, 5, red);
    }else if(minute < 20){
      //genWord(80, 2, 3, on);
      //genWord(80, 5, 10, on);
      generateWord(10, 6, 7, red);
      generateWord(10, 9, 14, red);
    }else if(minute < 25){
      //genWord(90, 0, 4, on); //Twenty on
      generateWord(11, 1, 5, red);
    }else if(minute < 30){
      //genWord(90, 0, 10, on); //25 on
      generateWord(12, 4, 14, red);
    }else{
      //genWord(100, 6, 10, on); //Half on
      generateWord(7, 1, 5, red);
    }
  }else{

      generateWord(9, 1, 4, red); //Meno

      if(diff != 0 && diff != 5) {

        if(minute % 5 != 0){
            generateWord(0, 2, 2, red);
            generateWord(0, 4, 4, black);
        }
        else{
          generateWord(0, 6, 6, black);
          generateWord(0, 8, 8, black);
          generateWord(0, 10, 10, black);
          generateWord(0, 12, 12, black);
          generateWord(0, 4, 4, black);
          generateWord(0, 2, 2, black);
        }
          if (diff == 2 || diff == 7) {
              generateWord(0, 6, 6, red);
		          generateWord(0, 8, 8, red);
		          generateWord(0, 10, 10, red);
              generateWord(0, 12, 12, black);
          } else if (diff == 3 || diff == 8) {
              generateWord(0, 6, 6, red);
		          generateWord(0, 8, 8, red);
		          generateWord(0, 10, 10, black);
              generateWord(0, 12, 12, black);
          } else if (diff == 4 || diff == 9) {
              generateWord(0, 6, 6, red);
		          generateWord(0, 8, 8, black);
		          generateWord(0, 10, 10, black);
              generateWord(0, 12, 12, black);
          } else if (diff == 1 || diff == 6) {
              generateWord(0, 6, 6, red);
		          generateWord(0, 8, 8, red);
		          generateWord(0, 10, 10, red);
              generateWord(0, 12, 12, red);
          }

          
      }
      if(minute > 55){
          generateWord(9, 1, 4, black);
          generateWord(0, 4, 4, red);
          generateWord(0, 2, 2, black);
      }else if(minute > 50){
          generateWord(12, 9, 14, red);
      }else if(minute > 45){
          generateWord(10, 1, 5, red);
      }else if(minute > 40){
          generateWord(10, 6, 7, red);
          generateWord(10, 9, 14, red);
      }else if(minute > 35){
          generateWord(11, 1, 5, red);
      }else if(minute > 30){
          generateWord(12, 4, 14, red);
      }
  }

  //secondi
  for(int i = 0; i < 13; i++){
    generateWord(i, 0, 0, black);
  }
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
void pixelOn(int pixel, uint32_t color) {
  strip.setPixelColor(pixel, color);
  strip.show();
}
void generateWord(int row, int min, int max, uint32_t color) {
  if(min == 0 && max == 0){
    for (int i = min; i <= (max - min); i++) {
      pixelOn(pixels[row][i], color);
    }
  }
  else{
    for (int i = min; i <= max; i++) {
      pixelOn(pixels[row][i], color);
    }
  }
}
