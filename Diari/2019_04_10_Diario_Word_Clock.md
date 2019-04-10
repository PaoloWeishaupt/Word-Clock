# Word Clock | Diario di lavoro - 10.04.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 10.04.2019

## Lavori svolti
Durante questa giornata abbiamo pincipalmente continuato con l'implementazione del word clock. Nel frattempo Gabriele ha anche riguardato la documentazione e aggiunto delle foto per la progettazione della scatola del word clock.  
Le seguenti sono le parti su cui si è lavorato maggiormente:
```arduino
//Illuminazione dei pallini
  diff = minute - int(minute / 10) * 10;

  //Minuti
  if(meno == false) {

    if(minute % 5 != 0){
        generateWord(0, 1, 1, white);
        generateWord(0, 2, 2, black);
    }
    else{
        generateWord(0, 1, 6, black);
    }

    if(diff != 0 && diff != 5) {
      //+ on
      //genWord(20, 6, 6, on);
        generateWord(0, 3, 3, white);
        generateWord(0, 4, 6, black);

```

Inoltre è stata implementata la funzione della parola "pausa".
```arduino
  int pausaHour = hour;

  //PAUSA
  //Mattino
  Serial.println(hour);
  Serial.println(minute);
  if((pausaHour == 9 && minute >= 50) || (pausaHour == 10 && minute >= 0 && minute < 2)){
    generateWord(0, 8, 12, green);
  }
  else if(pausaHour == 10 && minute >= 2 && minute <= 4){
    generateWord(0, 8, 12, red);
  }
  //Pomeriggio
  else if(pausaHour == 14 && minute >= 45 && minute <= 56){
    generateWord(0, 8, 12, green);
  }
  else if(pausaHour == 14 && minute >= 57 && minute <= 59){
    generateWord(0, 8, 12, red);
  }
  //Non pausa
  else{
    generateWord(0, 8, 12, black);
  }
```

| Orario | Lavori svolti |
| - | - |
|13:15 - 16:30 | Implementazione |

##  Problemi riscontrati e soluzioni adottate
Nessun problema riscontrato.
##  Punto della situazione rispetto alla pianificazione
In linea con la pianificazione.
## Programma di massima per la prossima giornata di lavoro
Continuazione implementazione, documentazione e revisione dei codici sorgente.