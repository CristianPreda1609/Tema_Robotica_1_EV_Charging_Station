# Tema_Robotica_1_EV_Charging_Station
# Stație de Încărcare pentru Vehicule Electrice

### Descrierea Task-ului

Proiectul simulează o stație de încărcare pentru vehicule electrice. Led-ul RGB indică disponibilitatea stației, aprinzându-se verde când stația este liberă și roșu când este ocupată.

LED-urile simple (L1, L2, L3, L4) reprezintă progresul de încărcare al bateriei:

- L1 = 25%
- L2 = 50%
- L3 = 75%
- L4 = 100%

Încărcarea este simulată prin aprinderea succesivă a LED-urilor, fiecare având un interval de 3 secunde între treceri. LED-ul care indică procentul curent de încărcare va clipi, LED-urile deja parcurse vor rămâne aprinse, iar celelalte vor rămâne stinse.

Funcționalitatea principală:

1. **Start încărcare:** La o apăsare scurtă a butonului de start, încărcarea va începe, iar LED-ul RGB va deveni roșu pentru a semnala ocuparea stației. LED-urile se vor aprinde succesiv, semnalizând progresul de încărcare.
   
2. **Stop încărcare:** O apăsare lungă (minim 1 secundă) a butonului de stop va întrerupe încărcarea. După stopare, toate LED-urile vor clipi de 3 ori pentru a indica întreruperea, iar LED-ul RGB va reveni la verde, semnalizând stația liberă.

3. **Finalizarea încărcării:** După ce toate LED-urile indică 100% încărcare, ele vor clipi simultan de 3 ori, iar apoi se vor stinge, semnalizând completarea încărcării și resetarea stației la starea inițială.

---

### Componente Utilizate

- **4x LED-uri:** Pentru simularea procentelor de încărcare (L1, L2, L3, L4)
- **1x LED RGB:** Pentru indicarea stării stației (liber/ocupat)
- **2x Butoane:** Pentru controlul start/stop al procesului de încărcare
- **8x Rezistoare:** (6x de 220Ω și 2x de 1KΩ)
- **Breadboard**
- **Linii de legătură**

---
## Explicarea codului
### Secțiunea 1: Definirea pinilor și variabilelor
**Atribuirea pinilor**
```
const int startButtonPin = 3;  
const int stopButtonPin = 2;   
const int redPin = 6;          
const int greenPin = 5;        
const int ledPins[] = {10, 9, 8, 7};
```  
**Variabile de timp**
```
volatile unsigned long lastLedBlinkTime = 0;   
volatile unsigned long lastLedTransitionTime = 0; 
const long ledInterval = 3000; 
const long blinkInterval = 500; 
volatile unsigned long stopButtonPressStart = 0; 
const long longPressDuration = 1000; 
```
**Variabile pentru debounce**
```
volatile unsigned long lastStartButtonPressTime = 0; 
volatile unsigned long lastStopButtonPressTime = 0;  
const long debounceDelay = 50; 
```
**Variabile de stare pentru butoane**
```
volatile bool isCharging = false;
volatile int currentLevel = 0;
volatile bool ledOn = false; 
```
### Secțiunea 2: Funcții de manipulare a LED-urilor
**Functie pentru intermitenta unui LED de nivel baterie**
```
void blinkLed(int pin) {
  if ((millis() - lastLedBlinkTime) >= blinkInterval) {
    ledOn = !ledOn;  
    digitalWrite(pin, ledOn ? LOW : HIGH); 
    lastLedBlinkTime = millis();
  }
}
```
**Functie pentru resetarea statiei la starea initiala**\
**Reseteaza tot: toate LED-urile de nivel baterie sunt oprite, RGB verde**
```
void resetStation() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);  
  }
  digitalWrite(redPin, HIGH);   
  digitalWrite(greenPin, LOW);  
  currentLevel = 0;
  isCharging = false;
}
```
**Functie pentru a porni incarcarea**\
**Schimba statia la ocupata (RGB rosu)**
```
void startCharging() {
  digitalWrite(greenPin, HIGH);  
  digitalWrite(redPin, LOW);     
  isCharging = true;
  currentLevel = 0;
  lastLedTransitionTime = millis(); 
  lastLedBlinkTime = millis();      
}
```
**Functie pentru a opri incarcarea**\
**Opreste incarcarea, face intermitenta la toate LED-urile de baterie de 3 ori, apoi reseteaza statia**
```
void stopCharging() {
  for (int i = 0; i <= 3; i++) {
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], HIGH);  
    }
    delay(500);  
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], LOW);  
    }
    delay(500);
  }
  resetStation();  
}
```
**Functie pentru a gestiona logica butonului de stop**\
**Verifica daca butonul de stop a fost apasat si statia se incarca**
```
void handleStopButton(unsigned long currentTime) {
  if (digitalRead(stopButtonPin) == LOW && (currentTime - lastStopButtonPressTime) > debounceDelay && isCharging) {
    if (stopButtonPressStart == 0) {
      stopButtonPressStart = currentTime;  
    }
    lastStopButtonPressTime = currentTime;  
  }

  if (digitalRead(stopButtonPin) == HIGH && stopButtonPressStart != 0) {
    stopButtonPressStart = 0;  
  }

  if (stopButtonPressStart != 0 && (currentTime - stopButtonPressStart) >= longPressDuration && isCharging) {
    stopCharging();  
    stopButtonPressStart = 0;  
  }
}
```
### Secțiunea 3: Configurarea inițială a pinilor
**Setari pentru pini**
```
void setup() {
  pinMode(startButtonPin, INPUT);  
  pinMode(stopButtonPin, INPUT);   
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  resetStation(); 
}
```
### Secțiunea 4: Funcția principală de control
```
void loop() {
  int startButtonState = digitalRead(startButtonPin);
  unsigned long currentTime = millis();
  ```
  **Incepe incarcarea cand butonul de start este apasat cu debounce**
  ```
  if (startButtonState == LOW && !isCharging && (currentTime - lastStartButtonPressTime) > debounceDelay) {  
    startCharging();
    lastStartButtonPressTime = currentTime;  
  }
  ```
 **Gestioneaza butonul de stop**
  ```
  handleStopButton(currentTime);
  ```
  **Procesul de incarcare**
  ```
  if (isCharging) {
    blinkLed(ledPins[currentLevel]);

    if ((millis() - lastLedTransitionTime) >= ledInterval) {
      digitalWrite(ledPins[currentLevel], HIGH); 
      
      currentLevel++;
      
      if (currentLevel >= 4) {
        stopCharging();
      } else {
        lastLedTransitionTime = millis();  
      }
    }
  }
}
```

### Poze ale setup-ului fizic

![Schema_Hardware](/Set-up_Fizic.jpg)

### Video cu funcționalitatea montajului fizic

*[Functionalitatea montajului fizic](https://youtube.com/shorts/DOYGgbuC3KE)*

---

### Schema Electrică

![Schema_Hardware](/Schema_Hardware.png)

