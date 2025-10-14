# RP2040 Zero PlatformIO Starter Pack

> A ready-to-go workspace for the Waveshare RP2040 Zero that combines PlatformIO, Arduino, FastLED, and practical buying guidance.

## Highlights

- Pre-configured PlatformIO project with multiple upload targets.
- Serial banner that surfaces the project repository URL for future reference.
- On-board LED and NeoPixel heartbeat to confirm the board is alive after flashing.
- Embedded pinout and mechanical reference images for quick lookup.

## Quick Start

1. Install [PlatformIO](https://platformio.org/install) (VS Code extension or CLI).
2. Clone this repo: `git clone https://github.com/Felipegalind0/rp2040-zero-docs-pinout-platformio-project`.
3. Open the `platformio-hello-world` folder in PlatformIO.
4. Pick your upload environment:
   - `pico_uf2`: drag-and-drop the generated UF2 onto the `RPI-RP2` volume.
   - `pico_picotool`: flash over USB with `picotool`.
   - `nanorp2040connect`: Arduino Nano RP2040 Connect board profile.
5. Build the firmware: `pio run -e pico_uf2`.
6. Upload it: `pio run -e pico_uf2 --target upload`.
7. Monitor serial at 115200 baud: `pio device monitor -b 115200`.

### Upload Mode Tips

- UF2 drag-and-drop disables USB serial. Flash the UF2, then press reset (or power cycle) to return to normal mode.
- Double-tapping reset re-enters mass-storage mode. Tap once to reboot into your sketch.
- When using `picotool`, let PlatformIO finish the upload and wait for it to reconnect to the serial monitor. If it does not, unplug and reconnect the cable.

### Runtime Behavior

- Prints a friendly banner and the GitHub repository URL once serial is ready.
- Toggles the built-in LED and NeoPixel as a 500 ms heartbeat.
- Emits "Hello from RP2040 over USB serial!" every second while powered.

### Sample Serial Output

```
Raspberry Pi Pico says hello!
USB serial link is ready.
Project repo: https://github.com/Felipegalind0/rp2040-zero-docs-pinout-platformio-project
Hello from RP2040 over USB serial! (count: 1)
Hello from RP2040 over USB serial! (count: 2)
Hello from RP2040 over USB serial! (count: 3)
...
```

## Value Comparison

| Bundle | Total price | Units | Price per unit | Best for | Link |
| --- | --- | --- | --- | --- | --- |
| Value pack | $16 | 5 | $3.20 | Stocking up for multiple builds | https://amzn.to/4h6JjyZ |
| Starter pack | $9 | 3 | $3.00 | Need a few backups without overbuying | https://amzn.to/3KOOvvt |
| Official Waveshare board | $9 | 1 | $9.00 | Supporting the creator for a one-off build | https://amzn.to/3KGDNaq |

Need just one board for a prototype? Support Waveshare directly with the official board. Planning more than two builds? One of the multi-pack bundles will stretch your budget further.

## Resources

- Project repository: https://github.com/Felipegalind0/rp2040-zero-docs-pinout-platformio-project
- Waveshare RP2040 Zero product page: https://www.waveshare.com/wiki/RP2040-Zero

## Reference Images

![RP2040 Zero Diagram](800px-900px-RP2040-Zero-details-7.jpg)
![RP2040 Zero Dimensions](800px-900px-RP2040-Zero-details-size.jpg)

## Project Goals

- Deliver a clean PlatformIO hello world project (Arduino framework + FastLED heartbeat).
- Provide pinout images that humans and LLMs can consume quickly.
- Share value-focused buying recommendations and inspirational project links.
- Act as the FOC MCU reference for an upcoming robot dog project.

## Waveshare RP2040 Zero Pinout Reference

Pin numbers match the Waveshare silkscreen (USB-C facing up with pin 1 in the top left corner).

### Power, Ground, and Analog Inputs

| Physical | Label | Role | Notes |
| --- | --- | --- | --- |
| 1 | 5V | Power | Feed from USB-C or external 5 V supply (tied to VSYS).
| 2, 29 | GND | Ground | Common ground reference for all peripherals.
| 30 | 3V3 | Power | 3.3 V regulator output (about 300 mA headroom).
| 31 | GP29 / ADC3 | Analog | 12-bit ADC channel 3.
| 32 | GP28 / ADC2 | Analog | 12-bit ADC channel 2.
| 33 | GP27 / ADC1 | Analog | 12-bit ADC channel 1.
| 34 | GP26 / ADC0 | Analog | 12-bit ADC channel 0.

### Digital GPIO Cheat Sheet

Every `GPx` pin supports digital I/O and PWM. Peripheral routes listed are RP2040 defaults and can be remapped.

| Physical | GPIO | Common peripheral routes | Notes |
| --- | --- | --- | --- |
| 3 | GP0 | SPI0 RX, I2C0 SDA, UART0 TX | Default SDA or UART0 TX. |
| 4 | GP1 | SPI0 CSn, I2C0 SCL, UART0 RX | Companion to GP0 for I2C or UART0. |
| 5 | GP2 | SPI0 SCK, I2C1 SDA | Primary SPI0 clock. |
| 6 | GP3 | SPI0 TX, I2C1 SCL | SPI0 COPI or I2C1 SCL. |
| 7 | GP4 | SPI0 RX, I2C0 SDA, UART1 TX | Alternate SDA or UART1 TX. |
| 8 | GP5 | SPI0 CSn, I2C0 SCL, UART1 RX | Alternate SCL or UART1 RX. |
| 9 | GP6 | SPI0 SCK, I2C1 SDA | Alternate SPI0 clock lane. |
| 10 | GP7 | SPI0 TX, I2C1 SCL | Alternate SPI0 COPI. |
| 11 | GP8 | SPI1 RX, I2C0 SDA, UART1 TX | Default SPI1 data in. |
| 12 | GP9 | SPI1 CSn, I2C0 SCL, UART1 RX | Default SPI1 chip-select. |
| 13 | GP10 | SPI1 SCK, I2C1 SDA | Default SPI1 clock. |
| 14 | GP11 | SPI1 TX, I2C1 SCL | Default SPI1 COPI. |
| 15 | GP12 | SPI1 RX, I2C0 SDA, UART0 TX | Alternate SPI1 input or UART0 TX. |
| 16 | GP13 | SPI1 CSn, I2C0 SCL, UART0 RX | Alternate SPI1 CS or UART0 RX. |
| 17 | GP14 | SPI1 SCK, I2C1 SDA | Flexible spare for SPI1 or I2C1. |
| 18 | GP15 | SPI1 TX, I2C1 SCL | Flexible spare for SPI1 or I2C1. |
| 19 | GP16 | GPIO, PWM | Drives the onboard WS2812B RGB LED. |
| 20 | GP17 | SPI0 CSn, I2C0 SCL, UART0 RX | Handy chip-select or UART0 RX. |
| 21 | GP18 | SPI0 SCK, I2C1 SDA | Popular for SPI sensors or displays. |
| 22 | GP19 | SPI0 TX, I2C1 SCL | Companion to GP18 for SPI0. |
| 23 | GP20 | I2C0 SDA, UART1 TX | Useful I2C or UART lane. |
| 24 | GP21 | I2C0 SCL, UART1 RX | Useful I2C or UART lane. |
| 25 | GP22 | GPIO, PWM | Free PWM-capable GPIO. |
| 26 | GP23 | GPIO, PWM | Free PWM-capable GPIO. |
| 27 | GP24 | GPIO, PWM | Free PWM-capable GPIO. |
| 28 | GP25 | GPIO, PWM | Spare PWM-capable GPIO. |

### Onboard Components

- USB-C connector for power, drag-and-drop flashing, and USB device projects.
- BOOT button to enter BOOTSEL/UF2 mode (hold BOOT, tap RESET, release BOOT).
- RESET button to restart without unplugging power.
- Addressable WS2812B RGB LED wired to `GP16` (exposed as `NEOPIXEL_PIN`).

### Mechanical Notes

- Board footprint: 23.5 mm × 18.0 mm.
- Pin pitch: 2.54 mm (0.1 in) for breadboard compatibility.
- Four corner mounting holes (about 1 mm radius) for standoffs or chassis mounting.

### Peripheral Summary

- Two SPI controllers (`SPI0`, `SPI1`).
- Two I2C controllers (`I2C0`, `I2C1`).
- Two UART controllers (`UART0`, `UART1`).
- Four 12-bit ADC channels (`ADC0` to `ADC3`).
- PWM capability on every GPIO via RP2040 PWM slices.

## Amazon Affiliate Link Disclaimer

As an Amazon Associate, I may earn a small commission from qualifying purchases. This helps support my work in providing useful resources and information. Thank you for your support!

## Machine-Readable Pin Data

```json
{
  "board_name": "Waveshare RP2040-Zero",
  "total_pins": 34,
  "onboard_led": {
    "pin_name": "GP16",
    "led_type": "WS2812 RGB"
  },
  "pins": [
    {"physical_pin": 1, "label": "5V", "type": "Power", "functions": []},
    {"physical_pin": 2, "label": "GND", "type": "Ground", "functions": []},
    {"physical_pin": 3, "label": "GP0", "type": "GPIO", "functions": ["SPI0 RX", "I2C0 SDA", "UART0 TX"]},
    {"physical_pin": 4, "label": "GP1", "type": "GPIO", "functions": ["SPI0 CSn", "I2C0 SCL", "UART0 RX"]},
    {"physical_pin": 5, "label": "GP2", "type": "GPIO", "functions": ["SPI0 SCK", "I2C1 SDA"]},
    {"physical_pin": 6, "label": "GP3", "type": "GPIO", "functions": ["SPI0 TX", "I2C1 SCL"]},
    {"physical_pin": 7, "label": "GP4", "type": "GPIO", "functions": ["SPI0 RX", "I2C0 SDA", "UART1 TX"]},
    {"physical_pin": 8, "label": "GP5", "type": "GPIO", "functions": ["SPI0 CSn", "I2C0 SCL", "UART1 RX"]},
    {"physical_pin": 9, "label": "GP6", "type": "GPIO", "functions": ["SPI0 SCK", "I2C1 SDA"]},
    {"physical_pin": 10, "label": "GP7", "type": "GPIO", "functions": ["SPI0 TX", "I2C1 SCL"]},
    {"physical_pin": 11, "label": "GP8", "type": "GPIO", "functions": ["SPI1 RX", "I2C0 SDA", "UART1 TX"]},
    {"physical_pin": 12, "label": "GP9", "type": "GPIO", "functions": ["SPI1 CSn", "I2C0 SCL", "UART1 RX"]},
    {"physical_pin": 13, "label": "GP10", "type": "GPIO", "functions": ["SPI1 SCK", "I2C1 SDA"]},
    {"physical_pin": 14, "label": "GP11", "type": "GPIO", "functions": ["SPI1 TX", "I2C1 SCL"]},
    {"physical_pin": 15, "label": "GP12", "type": "GPIO", "functions": ["SPI1 RX", "I2C0 SDA", "UART0 TX"]},
    {"physical_pin": 16, "label": "GP13", "type": "GPIO", "functions": ["SPI1 CSn", "I2C0 SCL", "UART0 RX"]},
    {"physical_pin": 17, "label": "GP14", "type": "GPIO", "functions": ["SPI1 SCK", "I2C1 SDA"]},
    {"physical_pin": 18, "label": "GP15", "type": "GPIO", "functions": ["SPI1 TX", "I2C1 SCL"]},
    {"physical_pin": 19, "label": "GP16", "type": "GPIO", "functions": ["WS2812 DIN"]},
    {"physical_pin": 20, "label": "GP17", "type": "GPIO", "functions": ["SPI0 CSn", "I2C0 SCL", "UART0 RX"]},
    {"physical_pin": 21, "label": "GP18", "type": "GPIO", "functions": ["SPI0 SCK", "I2C1 SDA"]},
    {"physical_pin": 22, "label": "GP19", "type": "GPIO", "functions": ["SPI0 TX", "I2C1 SCL"]},
    {"physical_pin": 23, "label": "GP20", "type": "GPIO", "functions": ["I2C0 SDA", "UART1 TX"]},
    {"physical_pin": 24, "label": "GP21", "type": "GPIO", "functions": ["I2C0 SCL", "UART1 RX"]},
    {"physical_pin": 25, "label": "GP22", "type": "GPIO", "functions": ["PWM"]},
    {"physical_pin": 26, "label": "GP23", "type": "GPIO", "functions": ["PWM"]},
    {"physical_pin": 27, "label": "GP24", "type": "GPIO", "functions": ["PWM"]},
    {"physical_pin": 28, "label": "GP25", "type": "GPIO", "functions": ["PWM"]},
    {"physical_pin": 29, "label": "GND", "type": "Ground", "functions": []},
    {"physical_pin": 30, "label": "3V3", "type": "Power", "functions": []},
    {"physical_pin": 31, "label": "GP29", "type": "ADC", "functions": ["ADC3"]},
    {"physical_pin": 32, "label": "GP28", "type": "ADC", "functions": ["ADC2"]},
    {"physical_pin": 33, "label": "GP27", "type": "ADC", "functions": ["ADC1"]},
    {"physical_pin": 34, "label": "GP26", "type": "ADC", "functions": ["ADC0"]}
  ]
}


























