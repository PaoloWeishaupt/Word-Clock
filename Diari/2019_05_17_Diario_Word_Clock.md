# Word Clock | Diario di lavoro - 17.05.2019

##### Paolo Weishaupt, Gabriele Alessi, Mattia Lazzaroni

### Canobbio, 17.05.2019

## Lavori svolti

Durante questa giornata Gabriele ha lavorato unicamente sulla documentazione (la parte dell'implementazione) mentre Mattia ha cominciato a realizzare la presentazione e successivamente ha lavorato sull'implementazione, sistemando tutti i problemi presenti nel codice in modo da poter superare tutti i test case. Paolo invece ha provato a realizzare il controllo dell'orario tramite i bottoni.


| Orario        | Lavori svolti   |
| ------------- | --------------- |
| 13:15 - 16:30 | Implementazione e Documentazione |

## Problemi riscontrati e soluzioni adottate
I problemi riscontrati oggi sono tutti nell'implementazione e per sistemarli abbiamo fatto del debug. Ad esempio dal minuto 35 al 36, la lettera "e" rimaneva accesa (dunque da "e trentacinque" a "e meno venti + 4"). Per sistemare questo abbiamo rimosso tutte le accensioni e gli spegnimenti della lettera "e" nel codice e li abbiamo messi in un'unica piccola parte di codie:
```arduino
//e
if (minute >= 5 && minute <= 35)
{
  generateWord(6, 10, 10, white);
}
else
{
  generateWord(6, 10, 10, black);
}
```
## Punto della situazione rispetto alla pianificazione
Oggi abbiamo terminato l'implementazione del word clock, dunque siamo in ritardo solamente nella documentazione e nel controllo dell'orologio tramite i bottoni.
## Programma di massima per la prossima giornata di lavoro
Nella prossima giornata di lavoro, Mattia concluderà i test case e li esporrà sulla doc. Paolo finirà ciò che ha iniziato oggi, ovvero il controllo dell'orario tramite dei bottoni. Gabriele invece continuerà unicamente a lavorare sulla documentazione.
