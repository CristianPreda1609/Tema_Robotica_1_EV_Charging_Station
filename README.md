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

### Poze ale setup-ului fizic

*(Adăugați imagini cu setup-ul hardware)*

---

### Video cu funcționalitatea montajului fizic

*(Adăugați link către un video cu funcționalitatea stației de încărcare)*

---

### Schema Electrică

![Schema_Hardware](/Schema_Hardware.png)

