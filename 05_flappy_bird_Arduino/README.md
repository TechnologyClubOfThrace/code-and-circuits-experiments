# Flappy Bird for Arduino Uno + 1.8" TFT ST7735

📁 Φάκελος: `05_flapy_bird_arduino/`

## Α. Προεπισκόπηση

<p align="center">
  <img src="images/flapy001.jpg" alt="Flapy Bird on Arduino with small TFT" width="500px" height="auto">
  <img src="images/flapy002.jpg" alt="Flapy Bird on Arduino with small TFT" width="500px" height="auto">
  <br>
  <em>Ολοκληρωμένη κατασκευή στον Σύλλογο Τεχνολογίας Θράκης</em>
  <br>
  <em>Ομάδα Κατασκευής: Δημήτρης Κ., Γιάννης Γ., Άρης Τ.</em>
</p>

---

## Β. Περιγραφή
Πρόκειται για έναν απλοποιημένο κλώνο του δημοφιλούς παιχνιδιού **Flappy Bird**, για τον μικροελεγκτή ATmega328P (Arduino Uno) και έγχρωμη οθόνη TFT 1.8".

Η συγκεκριμένη υλοποίηση βασίζεται στο αρχικό project του **Themistokle "mrt-prodz" Benetatos**, οι τροποποιήσεις μας αφορούν μια αλλαγή στην συνδεσμολογία του κουμπιού (button) για ευκολότερη υλοποίηση στο breadboard, επίσης έχουν γίνει κάποιες αλλαγές στον κώδικα γιατί στην αρχή μας έβγαζε 'scrambled' αποτελέσματα στην οθόνη.

* **Original Repo:** [ATmega328-Flappy-Bird-Clone](http://github.com/mrt-prodz/ATmega328-Flappy-Bird-Clone)
* **Website:** [mrt-prodz.com](http://mrt-prodz.com)

## Γ. Λειτουργίες & Software
Το παιχνίδι χρησιμοποιεί το πρωτόκολλο επικοινωνίας **SPI** για γρήγορη ανανέωση της οθόνης και βασίζεται στις παρακάτω βιβλιοθήκες:
* `Adafruit_GFX`
* `Adafruit_ST7735`

## Δ. Υλικά (Hardware)
Για την κατασκευή θα χρειαστείτε:
* **1x Arduino UNO**
* **1x ST7735 128x160 TFT display (SPI)**
* **1x Breadboard**
* **10x Jumper Wires (Dupont)**
* **1x Push Button**
* **🔋 Τροφοδοσία:** Μέσω USB (δυνατότητα για προσθήκη μπαταρίας 9V για φορητότητα).

## Ε. Συνδεσμολογία (Pinout)

### 1. Οθόνη TFT ST7735
| Οθόνη TFT | Arduino Uno | Περιγραφή |
| :--- | :--- | :--- |
| **VCC** | 5V | Τροφοδοσία |
| **GND** | GND | Γείωση |
| **CS** | D10 | Chip Select |
| **RESET** | D8 | Reset |
| **D/C** | D9 | Data/Command |
| **MOSI** | D11 | SPI Data |
| **SCK** | D13 | SPI Clock |
| **LED** | 3.3V | Backlight |

### 2. Έλεγχος (Button)
Έχουμε απλοποιήσει τη σύνδεση χρησιμοποιώντας την εσωτερική pull-up αντίσταση του Arduino:

| Σύνδεση | Pin |
| :--- | :--- |
| **Button Pin A** | D2 |
| **Button Pin B** | GND |

> **Σημείωση:** Στην αρχική έκδοση απαιτούνταν εξωτερική αντίσταση 10kΩ, αλλά στη δική μας εκδοχή το κουμπί συνδέεται απευθείας στο GND.

## 🚀 Οδηγίες Χρήσης
1. Πραγματοποιήστε τη συνδεσμολογία σύμφωνα με τους παραπάνω πίνακες.
2. Εγκαταστήστε τις βιβλιοθήκες της Adafruit από το Library Manager.
3. Ανεβάστε τον κώδικα στην πλακέτα σας.
4. Πιέστε το κουμπί για να ξεκινήσει το παιχνίδι!

---
## 🎬 Δείτε το σε δράση
<p align="center">
  <a href="https://www.youtube.com/shorts/n0aPS1Wc2bk">
    <img src="https://img.youtube.com/vi/n0aPS1Wc2bk/0.jpg" alt="Flappy Bird Gameplay">
  </a>
  <br>
  <em>Κάντε κλικ στην εικόνα για να δείτε το βίντεο στο YouTube.</em>
</p>


## ⚠️ Σημαντικές Σημειώσεις Hardware

> [!CAUTION]
> **ΣΥΜΒΑΤΟΤΗΤΑ ΤΑΣΗΣ (3.3V vs 5V)**
> Τα περισσότερα TFT modules αυτής της κατηγορίας λειτουργούν εσωτερικά στα **3.3V**. Παρόλο που πολλά "επιζούν" συνδεδεμένα απευθείας στις εξόδους 5V του Arduino Uno, αυτό δεν αποτελεί την ασφαλέστερη πρακτική για τη μακροζωία τους. 
> 
> Κάθε module είναι διαφορετικό! **Συμβουλευτείτε τις προδιαγραφές του δικού σας κατασκευαστή**. Αν η λογική της οθόνης σας είναι αποκλειστικά 3.3V, ενδέχεται να χρειαστείτε Level Shifters (5V -> 3.3V) για τα σήματα δεδομένων.

> [!TIP]
> **Button Logic:** Στον κώδικα έχει χρησιμοποιηθεί η ρύθμιση `INPUT_PULLUP` για το Pin 2. Αυτό σημαίνει ότι το κουμπί συνδέεται απευθείας στο GND χωρίς να χρειάζεται εξωτερική αντίσταση.

---
**Technology Club of Thrace** *Exploring Technology through Code & Circuits*
