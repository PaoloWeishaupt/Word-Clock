# Word Clock | Diario di lavoro - 17.04.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 17s.04.2019

## Lavori svolti
Durante questa giornata abbiamo lavorato sull'implementazione del word clock. In particolare ci sono stati problemi apparentemente hardware e nella prossima occasione sarebbe bello risolvere la situazione. Poi si potrà proseguire e concludere il word clock per implementare i pulsanti fisici per l'impostazione dell'orario anche offline.  
Il seguente è il codice usato per testare il display a 7 segmenti, dove viene fatto partite un semplice contatore.
```arduino
#include "SevSeg.h"
SevSeg sevseg; //Instantiate a seven segment controller object

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = false; // Default. Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg.setBrightness(90);
}

void loop() {
  static unsigned long timer = millis();
  static int deciSeconds = 0;
  
  if (millis() - timer >= 100) {
    timer += 100;
    deciSeconds++; // 100 milliSeconds is equal to 1 deciSecond
    
    if (deciSeconds == 10000) { // Reset to 0 after counting for 1000 seconds.
      deciSeconds=0;
    }
    sevseg.setNumber(deciSeconds, 1);
  }

  sevseg.refreshDisplay(); // Must run repeatedly
}
```
Poi useremo il display per impostare l'orario (ore e minuti) attraverso dei pulsanti che incrementano l'ora.  
Verso la fine della lezione abbiamo risolto il problema del word clock grazie all'intervento del docente. Come abbiamo immaginato c'erano problemi con la corrente e abbiamo cambiato il cavo dell'alimentatore.

| Orario | Lavori svolti |
| - | - |
|13:15 - 16:30 | Implementazione |

##  Problemi riscontrati e soluzioni adottate
Il word clock ha improvvisamente smesso di funzionare del tutto. I led non si accendono quando colleghiamo il Fishino, ma il problema sembra essere hardware piuttosto che software, visto che abbiamo provato a vedere lo stato della corrente con il multimetro e risultano esserci delle incongruenze.
##  Punto della situazione rispetto alla pianificazione
È necessario proseguire con il resto del progetto (documentazione), comunque non dovremmo essere in ritardo nel complessivo.
## Programma di massima per la prossima giornata di lavoro
Correzione problemi word clock, documentazione e revisione dei codici sorgente. Implementazione pulsanti fisici.