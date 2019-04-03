# Word Clock | Diario di lavoro - 29.03.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 29.03.2019

## Lavori svolti
Durante questa giornata Mattia e Paolo hanno continuato con l'implementazione del word clock. Però non ci sono stati particolari progressi, bensì sono stati riscontrati dei problemi che verranno prossimamente gestiti.  
Nel frattempo Gabriele si è occupato della gestione generale del progetto, riguardando la documentazione e il codice, rifacendo i diari e aiutando nell'implementazione.
```arduino
void printTime(int hour, int minute, int second) {
  int diff;
  boolean meno = false;

  if(minute < 35){
      meno = false;
  }else{
      meno = true;
      hour += 1;
  }

  if ((hour != 1 && hour != 13 && hour != 12 && hour != 24) || minute >= 35) {
    generateWord(1, 2, 5, red);
    generateWord(1, 7, 8, red);
  }
```
Questa è una delle parti di codice su cui si è maggiormente lavorato, dove si impostano i minuti da stampare e l'occasione in cui bisogna stampare "sono le".

| Orario | Lavori svolti |
| - | - |
|13:15 - 16:30 | Implementazione |

##  Problemi riscontrati e soluzioni adottate
Ci sono i seguenti problemi:
- Da perfezionare l'implementazione degli orari per mezzanotte e mezzogiorno ("Sono le mezzogiorno").
- Il dispositivo Fishino a un certo punto ha cominciato a scaldarsi emanando un odore di bruciato (non è la prima volta).
- I secondi ancora non funzionano perfettamente (all'avvio non sono accesi quelli correnti).
- È ancora da definire la questione dei minuti da 55 a 00 e i minuti che si aggiungono/diminuiscono (i +/- in alto della tabella).
- Ci sono altri problemi riguardo il Fishino: ad esempio a Mattia e Paolo e uscita la schermata di errore di Windows quando hanno attaccato il dispositivo nei loro PC.

Soluzioni:
- Da cambiare l'implementazione di "mezzanotte" e "mezzogiorno" aggiungendo delle condizioni proprie.
- Chiederemo al docente responsabile per quanto riguarda i problemi che il Fishino genera.
- Alla fine della giornata i secondi sono stati implementati correttamente.
- I minuti sotto a 55 funzionano tutti, la prossima volta definiremo i restanti.
##  Punto della situazione rispetto alla pianificazione
Sarebbe necessario continuare con la documentazione, ma con l'implementazione siamo avanti.
## Programma di massima per la prossima giornata di lavoro
Continuazione implementazione, documentazione e revisione dei codici sorgente.