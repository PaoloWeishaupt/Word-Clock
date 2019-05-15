# Word Clock | Diario di lavoro - 15.05.2019

##### Paolo Weishaupt, Gabriele Alessi, Mattia Lazzaroni

### Canobbio, 15.05.2019

## Lavori svolti

Oggi abbiamo continuato con l'implementazione e la documentazione. Gabriele ha finito il prototipo per controllare l'orario del word clock tramite pulsanti ed è pronto per essere implementato sul prodotto. Il metodo più utile per fare ciò è il seguente:
```arduino
int hourButton = 22;
int hourButtonState = 0;
int lastHourButtonState = 0;
int hourUnit = 0;
int hourTen = 0;

void incrementHour()
{
  hourButtonState = digitalRead(hourButton);
  if (hourButtonState != lastHourButtonState && hourButtonState)
  {
    if (hourUnit == 9)
    {
      hourUnit = 0;
      hourTen++;
    }
    else if (hourTen == 2 && hourUnit == 3)
    {
      hourUnit = 0;
      hourTen = 0;
    }
    else
    {
      hourUnit++;
    }
  }
  lastHourButtonState = hourButtonState;
}
```

Mattia e Paolo hanno come al solito lavorato sul word clock per risolvere ancora dei problemi rifinire il codice.  
Inoltre ci siamo portati avanti con la documentazione dell'implementazione arrivando fino alla parte del metodo setup() del file principale del progetto.

| Orario        | Lavori svolti   |
| ------------- | --------------- |
| 13:15 - 16:30 | Implementazione e Documentazione |

## Problemi riscontrati e soluzioni adottate
Ci sono i soliti problemi con i word clock che funziona quando vuole. Abbiamo risolto riformattando il codice e l'abbiamo ricaricato facendo un po' di debug anche con l'aiuto del docente.
## Punto della situazione rispetto alla pianificazione
In ritardo con la pianificazione e l'implementazione è in via di recupero.
## Programma di massima per la prossima giornata di lavoro
Conclusione implementazione, test, documentazione.