# Word Clock | Diario di lavoro - 27.02.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 27.02.2019

## Lavori svolti
Mattia e Paolo oggi insieme hanno testato il modulo RTC del Fishino stampando l'ora corrente su un display LCD: 
```arduino
lcd.init();
lcd.backlight();
lcd.clear(); //pulisce lo schermo
lcd.setCursor(0,0); //imposta il cursore nella prima riga e colonna,  
lcd.print(now.day(), DEC);     //stampo giorno numero in decimale
lcd.print('/');
lcd.print(now.month(), DEC); //stampo mese in decimale
lcd.print('/');
lcd.print(now.year(), DEC); //stampo anno in decimale
lcd.print(' ');
lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
lcd.setCursor(0,1); //imposta il cursore nella seconda riga e prima colonna.
//stampo orario in decimale
lcd.print(now.hour(), DEC);
lcd.print(':');
lcd.print(now.minute(), DEC);
lcd.print(':');
lcd.print(now.second(), DEC);
```
Per fare ciò hanno dapprima cercato di utilizzare la libreria scritta da Filippo e Matan nel secondo progetto del modulo, ma alla fine si sono documentati online. Sito consultato: https://www.wikiinfo.net/t2800-come-funziona-rtc-del-fishino-codice.  
Gabriele ha finito di analizzare i led progettando e caricando altri tipi di algoritmi e capendo meglio come funzionano gli indici dei led. Ad esempio:
```arduino
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
```

| Orario | Lavori svolti |
| - | - |
|13:15 - 16:30 | Progettazione |

##  Problemi riscontrati e soluzioni adottate
Abbiamo scoperto che per definire il colore non va bene usare il tipo di variabile int, ma bisogna usare uint32_t perchè altrimenti i colori verde e rosso non funzionano.
```arduino
void loop() {
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
```
##  Punto della situazione rispetto alla pianificazione
In linea con la pianificazione.
## Programma di massima per la prossima giornata di lavoro
Gabriele penserà come impostare i led per le parole.
Mattia e Paolo proveranno a stampare sul display LCD l´orario scritto in parole (e non in numeri).
