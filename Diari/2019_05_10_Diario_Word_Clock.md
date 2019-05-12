# Word Clock | Diario di lavoro - 10.05.2019

##### Paolo Weishaupt, Gabriele Alessi, Mattia Lazzaroni

### Canobbio, 10.05.2019

## Lavori svolti

Durante questa giornata abbiamo lavorato sull'implementazione del word clock. Gabriele ha implementato i pulsanti assieme al display 7 segmenti. Mattia e Paolo si sono occupati del codice per il word clock e hanno reso più leggibile il codice.

```arduino
//////////////////////////////////////////////////////////////////////////////////////////
// CONFIGURAZIONE VARIABILI
int piu[] = {0, 1, 1};
int simboliPiuPalliniMinuti[] = {0, 1, 6};
int menoSign[] = {0, 2, 2};
int primoMin[] = {0, 3, 3};
int primoSecondoMin[] = {0, 3, 4};
int primoSecondoTerzoMin[] = {0, 3, 5};
int tuttiMin[] = {0, 3, 6};
int secondoTerzoQuartoMin[] = {0, 4, 6};
int terzoQuartoMin[] = {0, 5, 6};
int quartoMin[] = {0, 6, 6};
int pausa[] = {0, 8, 12};
int eAccentata[] = {1, 1, 1};
int sono[] = {1, 2, 5};
int le[] = {1, 7, 8};
int luna[] = {1, 10, 14};
int mezzogiorno[] = {2, 1, 11};
int dueOre[] = {2, 12, 14};
int treOre[] = {3, 1, 3};
int quattroOre[] = {3, 5, 11};
int seiOre[] = {3, 12, 14};
int cinqueOre[] = {4, 1, 6};
int setteOre[] = {4, 10, 14};
int ottoOre[] = {5, 1, 4};
int noveOre[] = {5, 5, 8};
int dieciOre[] = {5, 10, 14};
int undiciOre[] = {6, 1, 6};
int e[] = {6, 10, 10};
int mezza[] = {7, 1, 5};
int mezzanotte[] = {7, 1, 10};
int cinqueMin[] = {8, 7, 12};
int meno[] = {9, 1, 4};
int dieciMin[] = {10, 1, 5};
int un[] = {10, 6, 7};
int quarto[] = {10, 9, 14};
int ventiMin[] = {11, 1, 5};
int venticinqueMin[] = {12, 4, 14};
int cinque[] = {12, 9, 14};
int sec1[] = {1, 0, 0};
int sec2[] = {2, 0, 0};
int sec3[] = {3, 0, 0};
int sec4[] = {4, 0, 0};
int sec5[] = {5, 0, 0};
int sec6[] = {6, 0, 0};
int sec7[] = {7, 0, 0};
int sec8[] = {8, 0, 0};
int sec9[] = {9, 0, 0};
int sec10[] = {10, 0, 0};
int sec11[] = {11, 0, 0};
int sec12[] = {12, 0, 0};

//////////////////////////////////////////////////////////////////////////////////////////

if(minute < 5){
  //genWord(80, 0, 0, off);//Past off
  generateWord(e[], black);
}
```

| Orario        | Lavori svolti   |
| ------------- | --------------- |
| 13:15 - 16:30 | Implementazione |

## Problemi riscontrati e soluzioni adottate

## Punto della situazione rispetto alla pianificazione

A livello di implementazione siamo praticamente giunti alla conclusione (mancano pochi test da svolgere), mentre con la documentazione siamo un po' indietro.

## Programma di massima per la prossima giornata di lavoro

Bisogna implementare il codice per collegare il word clock con la versione web e implementare i pulsanti e il display 7 segmenti con la matrice.
