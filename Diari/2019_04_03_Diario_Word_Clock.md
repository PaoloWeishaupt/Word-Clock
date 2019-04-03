# Word Clock | Diario di lavoro - 03.04.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 03.04.2019

## Lavori svolti
Durante questa giornata non ci sono stati particolari progressi, visto che inizialmente abbiamo avuto problemi a connettere il word clock e poi ci siamo bloccati in problemi simili a quelli riscontrati settimana scorsa.  
Nel frattempo Gabriele ha messo a posto tutti i diari così che siano presentati a regola d'arte.

```arduino
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
```
Ci sono stati problemi con questa porzione di codice, in quanto veniva segnalato che il RTC non era disponibile, invece esso risultava connesso correttamente e, dopo un po' di debug, siamo riusciti a risolvere l'inghippo.

| Orario | Lavori svolti |
| - | - |
|13:15 - 16:30 | Implementazione |

##  Problemi riscontrati e soluzioni adottate
I problemi sono bene o male quelli della volta scorsa. È necessario proseguire con la documentazione e risolvere questi problemi, perché ci siamo un po' bloccati sottovalutando la situazione.
##  Punto della situazione rispetto alla pianificazione
In linea con la pianificazione.
## Programma di massima per la prossima giornata di lavoro
Continuazione implementazione, documentazione e revisione dei codici sorgente.