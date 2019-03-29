# Word Clock | Diario di lavoro - 22.02.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 22.02.2019

## Lavori svolti
Mattia oggi ha creato il file per fare appunti in modo da annotare i siti visitati e scrivere le varie annotazioni, inoltre ha messo in ordine la documentazione (gli ultimi capitoli come la sitografia).  
Gabriele ha lavorato sulla striscia di led per cercare malfunzionamenti e provare a controllare i singoli led. Questa è una parte di codice utile per testare ciò:
```arduino
/**
   Accensione di un led di un colore definito.
*/
void pixelOn(int pixel, uint32_t color) {
  strip.setPixelColor(pixel, color);
  strip.show();
}
```
Paolo ha provato a collegarsi ad un server NTP per ricevere l'orario e lo ha fatto stampare sul monitor seriale:
```arduino
DateTime now = rtc.now(); //creo istanza ora/data
int hour = now.hour();
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
```

| Orario | Lavori svolti |
| - | - |
|13:15 - 16:30 | Progettazione |

##  Problemi riscontrati e soluzioni adottate
Se la striscia di led si imposta alla luminosità massima e si mostrano colori luminosi tende a far bloccare l'Arduino. Il valore che abbiamo scelto è 100 (max 255).
##  Punto della situazione rispetto alla pianificazione
In linea con la pianificazione.
## Programma di massima per la prossima giornata di lavoro
Provare a usare Fishino con NeoPixel e continuazione test del materiale.