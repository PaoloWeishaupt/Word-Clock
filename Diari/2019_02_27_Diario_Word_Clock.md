# Word Clock | Diario di lavoro - 27.02.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 27.02.2019

## Lavori svolti
Mattia e Paolo oggi inseme hanno testato il modulo RTC del Fishino stampando l'ora corrente su un display LCD. Per fare ciò all'inizio hanno dapprima cercato di utilizzare la libreria scritta da Filippo e Matan nel secondo progetto del modulo, ma alla fine si sono documentati online. Sito consultato: https://www.wikiinfo.net/t2800-come-funziona-rtc-del-fishino-codice.
Gabriele ha finito di analizzare i led.


|Orario        |Lavoro svolto                 |
|--------------|------------------------------|
|13:15 - 16:30 | Implementazione |

##  Problemi riscontrati e soluzioni adottate
Abbiamo scoperto che per definire il colore non va bene usare il tipo di variabile int, ma bisogna usare uint32_t perchè sennò i colori verde e rosso non funzionano.

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 5); // Red
  colorWipe(strip.Color(0, 255, 0), 5); // Green
  colorWipe(strip.Color(0, 0, 255), 5); // Blue
}

void colorWipe(uint32_t c, int wait) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
##  Punto della situazione rispetto alla pianificazione
In linea con la pianificazione.
## Programma di massima per la prossima giornata di lavoro
Gabriele penserà come impostare i led per le parole.
Mattia e Paolo proveranno a stampare sul display LCD l´orario scritto in parole (e non in numeri).
