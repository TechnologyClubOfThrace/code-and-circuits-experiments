# Lora module SX1278 with ds18b20

📁 Φάκελος: `03_lora_module_with_ds18b20/`

---

## Α. Περιγραφή
Μετάδοση θερμοκρασίας σε μεγάλες αποστάσεις με lora module SX1278.

---

## Β. Προεπισκόπηση
### Lora Module SX1278 με τις κεραίες στα 434MHz 2dBi

<p align="center">
  <img src="img/IMG_20260407_002450.jpg" alt="ds18b20 lora transmitter and receiver" width="500" height="auto">
</p>

---

## Γ. Σχηματικό Διάγραμμα του Κυκλώματος

<p align="center">
  <img src="img/schematic.jpg" alt="ds18b20 lora schematic" width="500" height="auto">
</p>

---

## Δ. Συνδεσμολογία Lora στο Arduino UNO

| LoRa SX1278 Module | Arduino UNO Board |
| :--- | :--- |
| **3.3V** | 3.3V |
| **Gnd** | Gnd |
| **En/Nss** | D10 |
| **G0/DIO0** | D2 |
| **SCK** | D13 |
| **MISO** | D12 |
| **MOSI** | D11 |
| **RST** | D9 |

> [!IMPORTANT]
> Συνδέστε το **ds18b20** στον ακροδέκτη **4** του Arduino UNO (transmitter). Η αντίσταση είναι στα **4,7kOhm**.

---

## Ε. Η παρουσίαση

<p align="center">
  <img src="img/transmitter_and_receiver.jpg" alt="ds18b20 lora transmitter and receiver" width="500" height="auto">
  <br>
  <em>Ομάδα Κατασκευής: Γιάννης Γ., Άρης Τ., Δημήτρης Κ.</em>
</p>

---
*Ολοκλήρωση της κατασκευής στον Σύλλογο Τεχνολογίας Θράκης*
