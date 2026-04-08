# arduino-mini-projects

Mini εκπαιδευτικά Arduino projects για αρχάριους, σχολικά εργαστήρια και μικρά demos με αισθητήρες, οθόνες και robot kits.

## Δομή repository

Κάθε project βρίσκεται σε δικό του αριθμημένο φάκελο:

- `01_*` για το πρώτο project
- `02_*` για το δεύτερο project
- κάθε φάκελος περιέχει το `.ino`, το δικό του `README.md` και τυχόν εικόνες/υλικό

Το `Backup/` χρησιμοποιείται μόνο για τοπικά αντίγραφα ασφαλείας και πλέον αγνοείται από το git.

## Projects

### 01. OLED DHT11 Mini Dashboard

Φάκελος: `01_oled_dht11_dashboard/`

- OLED SSD1306 128x64 με I2C
- DHT11 για θερμοκρασία και υγρασία
- απλό dashboard για αρχάριους
- timers με `millis()` χωρίς `delay()`

Αρχεία:

- `01_oled_dht11_dashboard/oled_dht11_dashboard.ino`
- `01_oled_dht11_dashboard/README.md`

### 02. Hosyond 4WD Master

Φάκελος: `02_hosyond_4wd_master/`

- Hosyond 4WD Smart Robot Car Kit
- modes: `MANUAL`, `LINE`, `AVOID`
- obstacle avoidance με ultrasonic + servo
- full scan και valley detection στη v2.7

Αρχεία:

- `02_hosyond_4wd_master/Hosyond_4wd_Master_TeacherEdition.ino`
- `02_hosyond_4wd_master/README.md`

## Πρόταση οργάνωσης για επόμενα projects

Για να μείνει το repo καθαρό, τα επόμενα projects καλό είναι να ακολουθούν το ίδιο μοτίβο:

- `03_project_name/`
- `04_project_name/`
- ένα κύριο `.ino` αρχείο
- ένα σύντομο `README.md` με υλικά, συνδεσμολογία και τι κάνει το sketch
- προαιρετικά εικόνες ή wiring photos μέσα στον ίδιο φάκελο

## Σημειώσεις

- License: MIT
- Για οδηγίες χρήσης του 4WD project, δες το `02_hosyond_4wd_master/README.md`
