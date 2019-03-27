
# Word Clock | Diario di lavoro - 27.03.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 27.03.2019

## Lavori svolti
Durante questa giornata ci siamo principalmente occupati dell'implementazione del word clock.  
Gabriele ha revisionato i vecchi codici così da avere un ambiente più ordinato e nel frattempo ha aiutato Mattia e Paolo che hanno continuato a occuparsi dell'orologio. Il prodotto sta funzionando sempre meglio e siamo arrivati al punto in cui c'è solo qualche dettaglio da mettere a posto, visto che l'orario viene a grandi linee mostrato in maniera corretta aggiornandosi automaticamente.
```arduino
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
```
Questa è la base che permette di accendere i led dei secondi: ogni 5 secondi viene acceso un led (12 led). Questa parte non funziona molto bene in quanto a volte i led lampeggiano o se ne accendono due alla volta, quindi prossimamente verrà revisionata e corretta.

|Orario        |Lavoro svolto                 |
|--------------|------------------------------|
|13:15 - 16:30 | Implementazione |

##  Problemi riscontrati e soluzioni adottate
Abbiamo risolto il problema del ritardo di un minuto semplicemente aggiungendolo manualmente.  
Ci sono ancora problemi per quanto riguarda l'uso di corrente da parte della striscia di led. Infatti se si accendono troppi led di un colore luminoso (bianco), il Fishino tende a bloccarsi anomalamente e i led prendono un colore più scuro. La soluzione sarebbe quella di fare in modo che non vengano mai generate situazioni che accenderebbero tanti led.
##  Punto della situazione rispetto alla pianificazione
In anticipo per quanto riguarda l'implementazione, è necessario proseguire con l'implementazione e concludere definitivamente alcuni capitoli come l'architettura del sistema (progettazione).
## Programma di massima per la prossima giornata di lavoro
Continuazione implementazione, documentazione e revisione dei codici sorgente.