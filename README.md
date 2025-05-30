# Reading INA219 Sensors

## Overview
- Supports up to 4 dividers.
- Arduino Pro Mini(5v/16MHz)
- 2.4k ressitor
- 10k ressitor
- 6.8k ressitor
- 1uF capacitor
- 0.1uF capacitor
- diode Zener 5.1V
- PTC 100mA
---

## Connection Diagram

### **RX to Arduino**
| RX Pin   | Arduino Pin         |
|----------|---------------------|
| GND      | GND                 |
| VDD      | RAW                 |
| Ext/E1   | D3                  |
| -        | D3 → R (2.4kΩ) → D4 |


---

### **Devices to Battery**
| INA219 Pin | Battery Terminal |
|------------|------------------|
| R1_IN      | Positive      |
| R1_OUT     | A0           |
| R1_OUT     | R2_OUT       |
| Didode Cathode     | A0       |
| Didode Athode     | GND       |
| C(0.1uF)     | A0       |
| C(0.1uF)     | GND       |
| C(1uF)     | A0       |
| C(1uF)     | GND       |
| GND        | Negative         |

---

### **Pins for Multiple Divider**
| Device | Analog Pin |
|--------|--------|
| 1      | A0    |
| 2      | A1    |
| 3      | A2    |
| 4      | A3    |
