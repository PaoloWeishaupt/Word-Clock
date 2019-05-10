# Word Clock | Diario di lavoro - 08.05.2019

##### Paolo Weishaupt, Gabriele Alessi, Mattia Lazzaroni

### Canobbio, 03.05.2019

## Lavori svolti

Durante questa giornata abbiamo lavorato sull'implementazione del word clock. Gabriele ha quasi finito di implementare i pulsanti con il display 7 segmenti. Mattia e Paolo hanno apportato delle modifiche al codice per renderlo più comprensibile. Hanno inoltre fixato alcuni bug inerenti al cambio di ora tra le 11, 12 e 13. Rispettivamente con le ore 23, 00 e 01.

```arduino
setNum(1, hourTen);
setNum(4, 4);	  setNum(2, hourUnit);
setNum(1, 1);	  hourButtonState = isClicked(hourButton);
setNum(2, 9);	  if (hourButtonState != lastHourButtonState && hourButtonState)
{
  if (hourUnit >= 0 && hourUnit <= 8 && hourTen < 2)
  {
    hourUnit++;
  }
  else if (hourUnit == 9 && (hourTen == 0 || hourTen == 1))
  {
    hourUnit = 0;
    hourTen++;
  }
  if (hourTen == 2)
  {
    if (hourUnit >= 0 && hourUnit <= 2)
    {
      hourUnit++;
    }
    else
    {
      hourUnit = 0;
      hourTen = 0;
    }
  }
  delay(50);
}
lastHourButtonState = hourButtonState;
}

bool isClicked(int button)
{
  return digitalRead(button);
  }	}
```

| Orario        | Lavori svolti   |
| ------------- | --------------- |
| 13:15 - 16:30 | Implementazione |

## Problemi riscontrati e soluzioni adottate

  Il problema relativo al non funzionamento del fishino non si è più ripresentato quindi lo consideriamo come risolto anche se non abbiamo apportato nessuna modifiche all'hardware.

## Punto della situazione rispetto alla pianificazione

  A livello di implementazione siamo praticamente giunti alla conclusione (mancano pochi test da svolgere), mentre con la documentazione siamo un po' indietro.

## Programma di massima per la prossima giornata di lavoro

  Nella prossima giornata di lavoro dovremmo  procedere con la documentazione e svolgere i test di funzionamento di tutti gli orari dell'orologio. Gabriele dovrà finire di implementare i bottoni nel 7 segmenti.
  
