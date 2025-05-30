# Reading Battery Voltage Using Divider

## Overview
- Supports up to 4 dividers.
- Arduino Pro Mini(5v/16MHz)
- R = 2.4k ressitor
- R1 = 10k ressitor
- R2 = 6.8k ressitor
- C1 = 0.1uF capacitor
- C2 = 1uF capacitor
- ZD = Zener Diode 5.1V
- PTC 100mA
---

## Connection Diagram

### **RX to Arduino**
| Ext/E1/E2 Port   | Arduino Pin         |
|----------|---------------------|
| -      | GND                 |
| +      | RAW                 |
| ~   | D3                  |
|         | D3 → R (2.4kΩ) → D4 |


---

### **Devices connection schematic**
| Node 1 | Node 2 |
|------------|------------------|
| R1_IN      | Battery Positive      |
| R1_OUT     | R2_IN       |
| R1_OUT/R2_IN     | A0          |
| R2_OUT     | GND       |
| GND        | Battery Negative |
| ZD Cathode     | A0       |
| ZD Anode     | GND       |
| C1(0.1uF)     | A0       |
| C1(0.1uF)     | GND       |
| C2(1uF)     | A0       |
| C2(1uF)     | GND       |
```
Battery Positive
     |
     R1
     |  
     +---------+-------+-------+------>A0
     |         |(A)    |       |
     R2        ZD      C1      C2
     |         |(C)    |       |
     +---------+-------+-------+------>GND
     |
Battery Negative

```
---

### **Pins for Multiple Divider**
| Device | Analog Pin |
|--------|--------|
| 1      | A0    |
| 2      | A1    |
| 3      | A2    |
| 4      | A3    |
