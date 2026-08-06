# ECET 251 Digital Communications — Final Project

**Camosun College — Trades & Technology — Winter 2026**

## Overview

A wireless link between two Arduinos over a 433 MHz ASK "transparent" RF module pair. The transmitter reads a potentiometer, Manchester-encodes the value along with a checksum, and sends it over the air at 300 baud. The receiver decodes the signal, verifies the checksum, and uses the value to set the brightness of an LED via PWM.

All data processing (encoding, framing, error detection) is done in software on the microcontrollers — the RF modules just carry the baseband signal.

## How it works

**Transmitter (`TX.ino`)**
- Reads a potentiometer on `A7`, mapped from the 10-bit ADC range down to 8 bits.
- On a button press (pin 3), builds a 3-byte message: `[length, pot value, checksum]`, where the checksum is the sum of the first two bytes.
- Sends the array via `man.transmitArray()` at `MAN_300` (300 baud) on pin 9.
- Green LED (pin 4) indicates the system is ready to transmit; blue LED (pin 5) flashes to confirm a transmission was sent.
- A 2-second cooldown between transmissions prevents button-mash spamming.

**Receiver (`RX.ino`)**
- Listens on pin 2 via `man.beginReceiveArray()`.
- On receipt of a complete 3-byte message, recalculates the checksum from the first two bytes and compares it to the third.
- If it matches, the potentiometer value is written out as a PWM signal to an LED on pin 5 (brightness control) and a success counter increments; otherwise the message is dropped and an error counter increments.
- Green status LED (pin 4) shows receiver activity, dimming briefly while a message is processed.

**Manchester encoding (`manchester.h` / `Manchester.cpp`)**
- Third-party library ([mchr3k/arduino-libs-manchester](https://github.com/mchr3k/arduino-libs-manchester)) used for the Manchester encode/decode and timer-based bit recovery on the ATmega328.
- Each transmission begins with a training preamble (14 zero-bits + a start bit) so the receiver's AGC can lock onto the signal before the actual payload is decoded.

## Files

| File | Description |
|---|---|
| `TX.ino` | Transmitter sketch (potentiometer read, button trigger, message send) |
| `RX.ino` | Receiver sketch (message decode, checksum check, LED PWM output) |
| `manchester.h` | Manchester library header (third-party, see credit above) |
| `Manchester.cpp` | Manchester library implementation (third-party, see credit above) |
| `ECET_251_FINAL_REPORT.docx` | Full project report/datasheet (block diagram, schematics, RF characteristics, calculations) |
| `ECET251_Final_Project_Overview_-_2026W.pdf` | Original assignment/rubric from the instructor |

## Hardware

- 2x Arduino (or compatible ATmega328-based board)
- 433 MHz ASK TX/RX module pair
- Potentiometer, pushbutton, status/output LEDs, pull-down and current-limiting resistors, smoothing capacitors

See the full report for the bill of materials, schematics, and measured RF characteristics (bandwidth, EIRP, minimum electric field at the receiver, bit error rate, etc.).
