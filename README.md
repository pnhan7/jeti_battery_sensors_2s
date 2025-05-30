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

```
Battery(+)
     |
     PTC(100ma)
     |
     R1(10k)
     |  
     +--------+--------+--------+--------> A0
     |        |(A)     |        |
     R2(6.8K) ZD(5.1V) C1(1uF)  C2(0.1uF)
     |        |(C)     |        |
     +--------+--------+--------+-------> GND
     |
Battery(-)

```
---

### **Pins for Multiple Divider**
| Device | Analog Pin |
|--------|--------|
| 1      | A0    |
| 2      | A1    |
| 3      | A2    |
| 4      | A3    |
