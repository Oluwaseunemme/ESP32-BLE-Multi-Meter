# ⚡ ESP32 BLE Multi-Meter

A custom **ESP32-based digital multimeter** capable of measuring **DC voltage, resistance, continuity, diode forward voltage, and capacitance**, with measurement results transmitted wirelessly to a **Flutter mobile application over Bluetooth Low Energy (BLE)**.

The project combines embedded firmware, analog measurement circuits, OLED display control, BLE communication, Flutter application development, KiCad PCB design, Proteus simulation, and object-oriented C++ firmware architecture.

---

## 📌 Project Overview

This project started as a collection of individual measurement experiments and evolved into a single multifunctional ESP32-based meter.

The meter can:

* Measure DC voltage
* Measure resistance
* Perform continuity testing
* Measure diode forward voltage
* Measure capacitance
* Display measurements locally on a **128×64 OLED**
* Send measurement results to a Flutter mobile application through **BLE**
* Provide a built-in resistor color-code calculator
* Calculate resistor series and parallel combinations
* Calculate capacitor series and parallel combinations
* Switch between measurement modes from the mobile application

The repository also contains the development stages of the project, including individual measurement programs, simulations, circuit designs, PCB layouts, and the final object-oriented firmware.

---

# 🧩 System Architecture

```text
                    ┌──────────────────────┐
                    │      Test Leads      │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │ Measurement Circuits │
                    │                      │
                    │ • Voltage            │
                    │ • Resistance          │
                    │ • Continuity         │
                    │ • Diode              │
                    │ • Capacitance        │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │        ESP32         │
                    │                      │
                    │ • ADC measurement    │
                    │ • Mode control       │
                    │ • Calculations       │
                    │ • OLED interface     │
                    │ • BLE communication  │
                    └───────┬───────┬──────┘
                            │       │
                     ┌──────▼───┐   │
                     │  OLED    │   │ BLE
                     │ 128×64   │   │
                     └──────────┘   │
                                    ▼
                          ┌──────────────────┐
                          │ Flutter Mobile   │
                          │      App         │
                          │                  │
                          │ • Live readings  │
                          │ • Calculators    │
                          │ • BLE interface  │
                          └──────────────────┘
```

---

# 🔧 Measurement Functions

## 1. Voltage Measurement

The ESP32 ADC is used to measure the scaled input voltage.

A resistor-divider network reduces the measured voltage to a level suitable for the ESP32 ADC, after which the firmware reconstructs the original voltage.

```text
Input Voltage
      │
      ▼
Resistor Divider
      │
      ▼
    ESP32 ADC
      │
      ▼
Voltage Calculation
      │
      ▼
OLED / BLE / Flutter
```

---

## 2. Resistance Measurement

Resistance is determined using a known reference resistor and the voltage developed across the unknown resistor.

Using the voltage-divider relationship:

$$
R_x = \frac{R_{ref}V_x}{V_{in}-V_x}
$$

The firmware converts the ADC measurement into resistance and automatically formats the result into an appropriate unit.

Example:

```text
35.30 Ω
497.47 Ω
12.00 kΩ
```

---

## 3. Continuity Testing

The continuity mode checks whether the resistance between the probes is below a defined threshold.

When continuity is detected, the meter can provide an audible indication through a buzzer while also displaying the result locally.

This makes the resistance measurement hardware usable as a dedicated continuity tester.

---

## 4. Diode Testing

The diode mode applies a controlled test condition to the diode and measures its forward voltage.

The measured voltage is then displayed as the diode's approximate forward voltage:

```text
Forward Voltage
      │
      ▼
   ESP32 ADC
      │
      ▼
Voltage Conversion
      │
      ▼
 OLED / Flutter
```

---

# 5. Capacitance Measurement

Capacitance is measured using the **RC charging time** of the capacitor under test.

For an RC circuit:

$$
V_C(t)=V_{CC}(1-e^{-t/RC})
$$

At approximately **63.2%** of the charging voltage:

$$
t=RC
$$

Therefore:

$$
\boxed{C=\frac{t}{R}}
$$

The firmware measures how long the capacitor takes to reach the 63.2% threshold and calculates its capacitance.

### Automatic Range Selection

Multiple charging resistors are used to accommodate different capacitor values.

```text
1 MΩ
100 kΩ
10 kΩ
1 kΩ
```

The firmware can select an appropriate resistor depending on the charging time, preventing large capacitors from requiring unnecessarily long measurements.

A transistor-controlled discharge path is also used to reset the capacitor before another measurement.

---

# 📱 Flutter BLE Application

The meter communicates with the mobile application using **Bluetooth Low Energy**.

The Flutter application provides:

### Live Measurement

Measurement values received from the ESP32 can be displayed on the phone.

```text
ESP32
  │
  │ BLE
  ▼
Flutter BLE Manager
  │
  ▼
Mobile UI
```

This allows the meter to function as both a standalone instrument and a wireless measurement device.

### Built-in Calculator

The application also contains several electronics calculators.

#### Resistor Color Code

The user can select resistor color bands and calculate the corresponding resistance and tolerance.

#### Resistor Series / Parallel

The application calculates equivalent resistance for:

* Series connections
* Parallel connections

#### Capacitor Series / Parallel

Equivalent capacitance can also be calculated for series and parallel capacitor configurations.

This makes the application useful not only for displaying measurements but also as a small electronics utility tool.

---

# 🖥️ Local OLED Interface

The meter uses a **128×64 OLED display** for local operation.

The OLED provides:

* Measurement values
* Measurement mode
* Meter status
* Local user feedback

The OLED allows the ESP32 meter to remain fully functional even when the phone is not connected.

---

# 🧠 Firmware Architecture

The project was developed progressively.

Individual measurement modes were first implemented and tested separately before being combined into the final meter firmware.

The repository therefore contains both:

### Individual Implementations

Separate firmware experiments for:

```text
Voltage
Resistance
Continuity
Diode
Capacitance
```

This made it easier to develop and debug each measurement system independently.

### Object-Oriented Implementation

The final firmware was reorganized into a more structured C++/OOP architecture.

The `meter_OOP` section contains the combined implementation where measurement operations are organized into reusable functions/classes rather than keeping every mode as an isolated program.

This also made it easier to integrate the measurement modes with the OLED interface and BLE communication.

---

# 🔌 Hardware Design

The hardware development went through schematic, simulation, breadboard, and PCB stages.

### KiCad

The repository contains the complete KiCad design files for the meter, including:

* Schematic
* PCB layout
* Component placement
* Routing
* PCB visualization / 3D representation

The PCB integrates the ESP32, measurement circuitry, display interface, power/control circuitry, and probe connections.

### Proteus

Proteus simulations were used during development to test individual measurement circuits and verify their behavior before hardware implementation.

---

# 🧪 Development Workflow

The project followed roughly this development process:

```text
Circuit Concept
      │
      ▼
Individual Arduino/ESP32 Test
      │
      ▼
Proteus Simulation
      │
      ▼
Breadboard Prototype
      │
      ▼
Measurement Verification
      │
      ▼
KiCad Schematic
      │
      ▼
PCB Design
      │
      ▼
ESP32 Firmware Integration
      │
      ▼
BLE Communication
      │
      ▼
Flutter Application
      │
      ▼
Final Multi-Meter
```

This repository therefore contains both the **final implementation and the development process behind it**.

---

# 📂 Repository Structure

```text
ESP32-Multi-Meter/
│
├── BLE_stuffs/
│   └── BLE experiments and communication code
│
├── displayInfo/
│   └── OLED/display-related experiments
│
├── get_capacitance_practice/
│   └── Early capacitance measurement experiments
│
├── getCap/
│   └── Capacitance measurement implementation
│
├── getCap_general/
│   └── General/extended capacitance implementation
│
├── getCap_schematic_simulation/
│   └── Capacitance circuit simulation/design
│
├── getDiode/
│   └── Diode measurement code
│
├── getOhm/
│   └── Resistance/continuity measurement
│
├── getVoltage/
│   └── Voltage measurement code
│
├── kicad_sch_pcb/
│   └── Complete KiCad schematic and PCB files
│
├── meter_OOP/
│   └── Combined object-oriented meter firmware
│
├── pcb_sch_img/
│   └── PCB and schematic images
│
└── Flutter_stuffs/
    └── Flutter mobile application
```

---

# 🛠️ Main Technologies

### Hardware

* ESP32
* 128×64 OLED
* ADC measurement circuits
* Resistor-divider networks
* Reference resistors
* Capacitor charging/discharging circuit
* Transistor-controlled discharge
* Buzzer
* Test probes

### Firmware

* C++
* Arduino framework
* ESP32 ADC
* GPIO
* Timers
* I²C
* Bluetooth Low Energy
* Object-Oriented Programming

### Software

* Flutter
* Dart
* KiCad
* Proteus
* Arduino IDE

---

# 📸 Project

The repository includes development and hardware documentation such as:

* KiCad schematic
* PCB layout
* PCB 3D visualization
* Proteus simulations
* Breadboard prototype
* ESP32 firmware
* Individual measurement implementations
* Combined OOP firmware
* Flutter application screenshots
* BLE development experiments

---

# 🎯 What This Project Demonstrates

This project brings together several areas of electronics and software engineering:

* Analog measurement
* ADC signal processing
* Voltage-divider calculations
* RC transient analysis
* Automatic measurement ranging
* Embedded C++
* Object-oriented firmware design
* OLED interfacing
* BLE communication
* Flutter application development
* PCB design
* Circuit simulation
* Hardware prototyping
* Embedded-to-mobile communication

Rather than relying on a single integrated multimeter IC, the project explores how common electronic measurement techniques can be implemented directly using a microcontroller and custom analog circuitry.

---

# 🚀 Future Improvements

Possible future improvements include:

* [ ] Improved measurement accuracy and calibration
* [ ] AC voltage measurement
* [ ] Current measurement
* [ ] Inductance measurement
* [ ] Capacitor ESR measurement
* [ ] Better input protection
* [ ] More measurement ranges
* [ ] Improved PCB enclosure
* [ ] Data logging and measurement history
* [ ] Exporting measurements from the Flutter application
* [ ] Rechargeable battery/power management
* [ ] Improved BLE reliability and connection handling

---

# 📜 License

This project is intended primarily for educational, experimental, and personal development purposes.

Feel free to explore, modify, and build upon the project.
