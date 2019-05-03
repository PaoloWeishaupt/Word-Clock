# Word Clock | Diario di lavoro - 03.05.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 03.05.2019

## Lavori svolti
Durante questa giornata abbiamo lavorato sull'implementazione del word clock.  
Siamo prossimi nel fare i test (almeno su quello che funziona) e quindi fare una revisione generale.
Paolo e Mattia hanno lavorato su ciò, nel frattempo Gabriele ha continuato con l'implementazione dei pulsanti fisici.
```arduino
int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int D1 = 9;
int D2 = 10;
int D3 = 11;
int D4 = 12;

void setup() {
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
}

void loop() {
  digitalWrite(D1, LOW);
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, HIGH);
  digitalWrite(pinC, LOW);
  digitalWrite(pinD, LOW);
  digitalWrite(pinE, HIGH);
  digitalWrite(pinF, HIGH);
  digitalWrite(pinG, HIGH);
}
```
![7Seg](img/7Seg.png)

https://create.arduino.cc/projecthub/SAnwandter1/programming-4-digit-7-segment-led-display-2d33f8  
http://www.circuitbasics.com/arduino-7-segment-display-tutorial/  
Grazie a questi elementi siamo riusciti a capire il funzionamento dei 4 display 7 segmenti, quindi durante la prossima occasione si potrebbe pensare di concludere ciò aggiungendo i pulsanti cambiando l'orario.

| Orario | Lavori svolti |
| - | - |
|13:15 - 16:30 | Implementazione |

##  Problemi riscontrati e soluzioni adottate
Inizialmente, dal software di Arduino collegando il fishino, non riuscivamo a trovare nessuna porta. Per riuscire a trovare la porta abbiamo cambiato la board per poi rimettere quella corretta. A quel punto l'IDE è riuscito a trovare la porta corretta.
Tuttavia al momento andando ad avviare il nostro programma il word clock non da segni di vita seppur nella console di Arduino esca scritto "FishDude done flashing".
##  Punto della situazione rispetto alla pianificazione
A livello di implementazione siamo praticamente giunti alla conclusione (mancano pochi test da svolgere), mentre con la documentazione siamo un po' indietro.
## Programma di massima per la prossima giornata di lavoro
Nella prossima giornata di lavoro dovremmo sistemare il problema del word clock che non risponde, procedere con la documentazione e svolgere i test di funzionamento di tutti gli orari dell'orologio.