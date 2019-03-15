
# Word Clock | Diario di lavoro - 15.03.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 15.03.2019

## Lavori svolti
Mattia si è occupato di stampare le ore e i secondi tramite un solo metodo invece di usare un metodo per ogni ora.
Paolo e Gabriele grazie all'aiuto del docente Barchi sono riusciti a collegare come da documentazione il Fishino e Adafruit.
Nel frattempo stiamo mettendo in ordine la striscia di led, infatti il docente ci ha procurato i fogli con le parole stampati correttamente e una lastra trasparesente di sostenimento.  
A fine giornata il risualtato è stato mostrare l'ora correttamente e parzialmente anche i secondi.  
Durante le prossime occasioni sarebbe necessario proseguire con la documentazione così da chiudere definitivamente il capitolo di progettazione e concentrarci di più sull'implementazione.  
Il seguente è il metodo utile per definire la parola da accendere (in altri metodi si prende l'orario dal RTC e tramite delle condizioni si ottiene il valore voluto):
```arduino
void generateWord(int row, int min, int max, uint32_t color) {
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
```

|Orario        |Lavoro svolto                 |
|--------------|------------------------------|
|13:15 - 16:30 | Implementazione |

##  Problemi riscontrati e soluzioni adottate
Nella giornata di oggi non sono stati riscontrati particolari problemi.
##  Punto della situazione rispetto alla pianificazione
In anticipo per quanto riguarda l'implementazione.
## Programma di massima per la prossima giornata di lavoro
Continuazione dell'implementazione degli orari con i led.