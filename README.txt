SMART THERMAL MONITOR
=====================

Proiect pentru disciplina Proiectare cu Microprocesoare (PM)
Realizat cu Arduino UNO, DS18B20, LCD I2C, buzzer, potențiometru și 4 LED-uri controlate prin registre


1. DESCRIERE GENERALĂ
----------------------

Acest proiect implementează un sistem de monitorizare termică cu alertă vizuală și acustică, capabil să funcționeze atât în regim real (citind temperatura din mediul înconjurător), cât și în regim de testare (simulare temperatură prin potențiometru). Afișajul este realizat pe un LCD I2C, iar avertizarea în caz de supratemperatură se face printr-un buzzer și un ansamblu de 4 LED-uri controlate direct prin registre (DDRx / PORTx).


2. FUNCȚIONALITĂȚI IMPLEMENTATE
-------------------------------

- Citirea temperaturii reale cu senzor DS18B20
- Simulare temperatură în mod test cu potențiometru
- Comutator fizic între mod real și test
- Afișaj LCD 1602 I2C pentru temperatură + starea sistemului
- Buzzer activat la depășirea pragului de 28°C
- LED-uri animate secvențial în regim de alertă, controlate direct prin registre
- Buton ON/OFF cu fire pentru activarea/dezactivarea sistemului
- Transmiterea datelor prin Serial Monitor pentru debugging


3. PARCURSUL PROIECTULUI
-------------------------

- **S10 (28 Apr – 2 Mai):** Confirmarea temei și testare senzor DS18B20 + afișaj LCD
- **S11 (5 – 9 Mai):** Implementare mod test cu potențiometru + afișaj dinamic
- **S12 (12 – 16 Mai):** Adăugare buzzer + LED-uri cu `digitalWrite()`, test sistem ON/OFF
- **S13 (19 – 23 Mai):** Refactorizare LED-uri cu registre directe; finalizare logică alertă
- **S14 (26 – 30 Mai):** Prezentare, documentație, urcare pe GitHub, generare schemă și wiki

---

4. CE NU A FUNCȚIONAT / FEEDBACK REALIST
-----------------------------------------

- Am încercat inițial să folosim un senzor BMP280 pentru citirea presiunii, dar acesta nu a fost detectat pe magistrala I2C. Probabil era o versiune SPI-only sau modulul era defect.
- S-a testat și controlul unui MOSFET pentru un element de încălzire, dar din lipsa unei surse externe adecvate de 12V și a lipsei disipării termice, funcționalitatea a fost lăsată deoparte pentru demo.
- În prima fază LED-urile au fost controlate cu `digitalWrite()`, dar pentru eficiență și punctaj, s-a trecut la control pe registre (cu succes).
- Am lucrat incremental, testând fiecare componentă înainte de integrarea în sistemul final.
- Am avut dificultăți minore la detecția stării butonului cu fire, dar am rezolvat folosind `INPUT_PULLUP` și detectare de tranziție.

---

5. CONCLUZII
------------

Proiectul demonstrează integrarea practică a conceptelor din laboratoarele de PM: GPIO, ADC, I2C, UART și lucru direct cu registre. Sistemul este complet funcțional, clar documentat și modular. Este ușor de extins și scalabil pentru aplicații reale (ex: cu Wi-Fi sau aplicație mobilă).

Repo GitHub: [https://github.com/alexciobotea21/Smart-Thermal-Monitor]

