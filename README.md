# Arduino & ESP32 Mini Projects

<table>
  <tr>
    <td width="190" valign="top">
      <img src="docs/assets/makerlab-3.png" alt="MakerLab logo" width="170" />
    </td>
    <td valign="top">
      Μικρά εκπαιδευτικά Arduino και ESP32 projects σχεδιασμένα για αρχάριους, σχολικά εργαστήρια και μικρά demos με αισθητήρες, οθόνες και robot kits. Στόχος του repository είναι η απλή, κατανοητή και πρακτική εισαγωγή στον προγραμματισμό μικροελεγκτών μέσα από έτοιμα αλλά επεκτάσιμα παραδείγματα στην Ελληνική γλώσσα.
    </td>
  </tr>
</table>

## Projects

---

### 01 — OLED DHT11 Mini Dashboard

📁 Φάκελος: `01_oled_dht11_dashboard/` — Απλό περιβαλλοντικό dashboard για Arduino με OLED SSD1306 και αισθητήρα DHT11, που δείχνει θερμοκρασία/υγρασία με καθαρό UI και λογική non-blocking (`millis()`), ιδανικό ως πρώτο βήμα σε αισθητήρες και displays.

---

### 02 — Hosyond 4WD Master (Teacher Edition)

📁 Φάκελος: `02_hosyond_4wd_master/` — Ολοκληρωμένο 4WD robot car project με manual control (IR/Bluetooth/Serial), line tracking και obstacle avoidance με ultrasonic + servo σε fail-safe λογική, κατάλληλο για βασικές έννοιες ρομποτικής και mode-based προγραμματισμού.

---

### 03 — LoRa SX1278 με DS18B20

📁 Φάκελος: `03_lora_module_with_ds18b20/` — Ασύρματη τηλεμετρία θερμοκρασίας με αρχιτεκτονική transmitter/receiver, LoRa SX1278 στα 434MHz και αισθητήρα DS18B20, χρήσιμη για εισαγωγή σε LoRa επικοινωνία, SPI και πρακτικά σενάρια απομακρυσμένης μέτρησης.

---

### 04 — Brent Crude Oil Live Tracker (ESP32-C3)

📁 Φάκελος: `04_oil_price_tracker/` — IoT εφαρμογή με ESP32-C3 που συνδέεται σε WiFi, λαμβάνει live τιμή Brent από API, κάνει JSON parsing και την εμφανίζει σε OLED 128x64, δείχνοντας στην πράξη ροές πραγματικών δεδομένων και API-driven embedded σχεδίαση.

---

### 05 — Flappy Bird για Arduino Uno

📁 Φάκελος: `05_flappy_bird_Arduino/` — Mini game project τύπου Flappy Bird για Arduino Uno με TFT ST7735 μέσω SPI και χειρισμό με ένα κουμπί, ιδανικό για κατανόηση game loop, input handling και βασικής γραφικής απόδοσης σε μικροελεγκτή.

---

### 06 — R2inoD2ino (Voice Robot)

📁 Φάκελος: `06 R2inoD2ino/` — Δημιουργικό voice robot με Arduino που ενώνει αναγνώριση φωνής, ήχο, servo και LED μέσα σε papercraft κατασκευή, προσφέροντας πρακτική εξάσκηση σε συνδυασμό πολλών modules και event-based λογική.

---

### 07 — Dino Game (Chrome Offline) για ESP

📁 Φάκελος: `07_dino_game_ESP/` — Endless runner τύπου Chrome Dino για ESP με ενσωματωμένη TFT ST7789 και button input, κατάλληλο για εκμάθηση game state λογικής, rendering γραφικών και workflow compile/upload σε ESP32.

---
