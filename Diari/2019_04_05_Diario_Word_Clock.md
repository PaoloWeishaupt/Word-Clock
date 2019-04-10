# Word Clock | Diario di lavoro - 05.04.2019
##### Gabriele Alessi, Mattia Lazzaroni, Paolo Weishaupt
### Canobbio, 05.04.2019

## Lavori svolti
Durante questa giornata abbiamo inizialmente risolto il problema dei colori luminosi dei led. Infatti i led tendevano a un colore scuro quando si volevano accendere tutti di bianco. Abbiamo risolto collegando l'inizio della striscia con la fine perché i cavi non sopportavano la tensione provocando un calo.  
Mattia e Paolo si sono occupati appunto su ciò, mentre Gabriele ha continuato con la documentazione, dove ha scritto le librerie utilizzate: 
- Librerie Adafruit 
    - Adafruit NeoPixel  
Libreria che presenta funzioni basate per il controllo di strisce di led RGB prodotte da Adafruit. Nella libreria ci sono funzionalità che sono necessarie per lo sviluppo del progetto, come ad esempio l’accensione di un led di un certo colore.  
Link: https://github.com/adafruit/Adafruit_NeoPixel
    - RTClib  
Gestione di un dispositivo RTC in un ambiente di sviluppo Arduino. La libreria presenta metodi utili per ottenere l’orario corrente e tutti i suoi componenti (anno, mese, giorno, ore, minuti, secondi).  
Link: https://github.com/adafruit/rtclib
- Librerie Arduino e Fishino
    - WiFi  
Grazie a questa libreria, il dispositivo sarà in grado di connettersi a internet. Ciò serve per potersi connettere a un server e ricevere l’orario via wireless.  
Link: https://www.arduino.cc/en/Reference/WiFi
    - Fishino  
Se si usa un Fishino è necessario scaricare la libreria dal sito ufficiale per integrarla in Arduino IDE. Questa libreria è essenziale per il funzionamento del dispositivo e integrare i vari moduli come WiFi e RTC.  
Link: https://www.fishino.it/download-libraries-it.html


| Orario | Lavori svolti |
| - | - |
|13:15 - 16:30 | Implementazione |

##  Problemi riscontrati e soluzioni adottate
A inizio giornata abbiamo risolto il problema dell'accensione di tanti led di un colore luminoso.
##  Punto della situazione rispetto alla pianificazione
In linea con la pianificazione.
## Programma di massima per la prossima giornata di lavoro
Continuazione implementazione, documentazione e revisione dei codici sorgente.