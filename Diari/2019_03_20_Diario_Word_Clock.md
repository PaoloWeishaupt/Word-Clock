# Word Clock | Diario di lavoro - 20.03.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 20.03.2019

## Lavori svolti
Durante questa giornata abbiamo principalmente continuato a lavorare sull'implementazione dell'orologio e sulla documentazione.  
Gabriele nel corso della lezione ha svolto la documentazione, principalmente si è concentrato sull'abstact che fino ad ora era troppo corto ed incompleto.  Questa è una parte dell'abstract: "*This strip is programmed in Arduino so that the led that form the words of the current time are turned on*".  
Mattia e Paolo si sono occupati invece dell'implementazione del word clock. Inizialmente hanno sistemato delle piccole parti di codice, rimuovendo ciò che non era necessario. Fatto ciò hanno iniziato l'implementazione dei minuti dell'orologio, compreso il "più" ed il "meno" a differenza dei minuti attuali.  

| Orario | Lavori svolti |
| - | - |
|13:15 - 16:30 | Implementazione |

##  Problemi riscontrati e soluzioni adottate
I led dei secondi lampeggiavano 2 volte al secondo invece che 1 volta, per sistemare ciò abbiamo aggiunto un delay di 1 secondo.
Inoltre ci siamo accorti che l'orario ritornato dal fishino era indietro di un minuto rispetto all'orario del computer. Abbiamo quindi optato per utilizzare del codice esistente negli esempi di Arduino per definire l'orario.
##  Punto della situazione rispetto alla pianificazione
In anticipo per quanto riguarda l'implementazione, giusti invece per la documentazione.
## Programma di massima per la prossima giornata di lavoro
Continuazione dell'implementazione degli orari con i led.