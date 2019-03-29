# Word Clock | Diario di lavoro - 13.03.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 13.03.2019

## Lavori svolti
Durante questa giornata abbiamo continuato a testare i led e abbiamo trovato un modo migliore per definire la matrice che li identifica. Infatti il foglio in cui sono scritti gli orari andava girato in orizzontale e abbiamo ritenuto più vantaggioso cambiare la matrice (`pixels[13][15]`) invece del foglio con le parole.  
La seguente è la nuova matrice:
```arduino
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
```
Infine siamo parzialmente riusciti a stampare l'ora (solo l'ora) nei led, quindi prossimamente lavoreremo sul resto della tabella.  
Questo è l'implementazione fatta:
```arduino
String printTime(int hour, int minute, int second){
   String time = "";
   if(hour != 1 && hour != 13){
      time += "Sono le ";
    if(hour == 2 || hour == 14){
        time += "due ";
    }else if(hour == 3 || hour == 15){
        time += "tre ";
    }else if(hour == 4 || hour == 16){
        time += "quattro ";
    }else if(hour == 5 || hour == 17){
        time += "cinque ";
    }else if(hour == 6 || hour == 18){
        time += "sei ";
    }else if(hour == 7 || hour == 19){
        time += "sette ";
    }else if(hour == 8 || hour == 20){
        time += "otto ";
    }else if(hour == 9 || hour == 21){
        time += "nove ";
    }else if(hour == 10 || hour == 22){
        time += "dieci ";
    }else if(hour == 11 || hour == 23){
        time += "undici ";
    }else if(hour == 12 || hour == 24){
        time += "dodici ";
    }
   }
   else{
      time += "È la una";
   }
   return time;
}
```
Il risultato di questo metodo viene usato nel metodo `loop()` che stampa la parola nei led tramite delle condizioni che definiscono i led da accendere.

| Orario | Lavori svolti |
| - | - |
|13:15 - 16:30 | Progettazione e implementazione |

##  Problemi riscontrati e soluzioni adottate
Nella giornata di oggi non sono stati riscontrati particolari problemi.
##  Punto della situazione rispetto alla pianificazione
In anticipo per quanto riguarda l'implementazione, ma la progettazione verrà conclusa la prossima lezione come pianificato.
## Programma di massima per la prossima giornata di lavoro
Continuazione dell'implementazione degli orari con i led.