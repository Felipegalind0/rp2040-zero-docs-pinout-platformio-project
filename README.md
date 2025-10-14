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
4. Pick your preferred upload environment:
  - `pico_uf2`: drag-and-drop via the `RPI-RP2` storage volume.
  - `pico_picotool`: flash over USB with `picotool`.
  - `nanorp2040connect`: Arduino Nano RP2040 Connect board profile.
5. Build the firmware: `pio run -e pico_uf2`
6. Upload it: `pio run -e pico_uf2 --target upload`
7. Monitor the serial output at 115200 baud: `pio device monitor -b 115200`

### Upload Mode Gotcha
- When the board mounts as `RPI-RP2` for UF2 drag-and-drop, the USB serial interface is intentionally disabled. Flash the UF2 and press reset (or power cycle) to return to normal mode with working serial.
- If you double-tap reset you may land in the mass-storage mode again—tap once to reboot into your sketch.
- When using `picotool`, finish the upload and wait for PlatformIO to reconnect to the serial monitor automatically; if it does not, unplug and reconnect the cable to leave the bootloader mode.

### Runtime Behavior
- Prints a friendly banner and the GitHub repository URL once serial is ready.
- Toggles the built-in LED and NeoPixel as a heartbeat (500 ms cadence).
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
| Official Waveshare board | $9 | 1 | $9.00 | Supporting the creator when prototyping a one-off | https://amzn.to/3KGDNaq |

Need just one board for a prototype? Support Waveshare directly with the official board. Planning more than two builds? One of the multi-pack bundles will stretch your budget further.

## Resources
- Project repository: https://github.com/Felipegalind0/rp2040-zero-docs-pinout-platformio-project
- Waveshare RP2040 Zero product page: https://www.waveshare.com/wiki/RP2040-Zero

## Reference Images
![RP2040 Zero Diagram](800px-900px-RP2040-Zero-details-7.jpg)

![RP2040 Zero Dimensions](800px-900px-RP2040-Zero-details-size.jpg)

## Project Goals
Deliver a one-stop reference for the RP2040 Zero that combines:
- A clean PlatformIO hello world project (Arduino framework + FastLED heartbeat).
- Pinout images that humans and LLMs can consume quickly.
- Value-focused buying recommendations and inspirational project links.

This board will act as the FOC MCU for an upcoming robot dog project.

## Amazon Affiliate Link Disclaimer
As an Amazon Associate, I may earn a small commission from qualifying purchases. This helps support my work in providing useful resources and information. Thank you for your support!

