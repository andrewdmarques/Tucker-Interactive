# ESP32 Patterned Output Sketch

This repository includes an ESP32 Arduino sketch (`Esp32Pattern.ino`) that outputs a repeating HIGH/LOW signal on a chosen GPIO pin. The pattern is:

- **HIGH** for 0.3 seconds
- **LOW** for 0.2 seconds
- **HIGH** for 15 minutes
- **Repeat indefinitely**

## How It Works

- The sketch uses non-blocking timing (via `millis()`) to alternate the pin's state according to the pattern.
- The output pin is chosen for safe booting (`GPIO 23`).
- The timing logic is implemented via a simple state machine with three phases.

## Timing Phases

| Phase            | Pin State | Duration     |
|------------------|-----------|-------------|
| HIGH (short)     | HIGH      | 0.3 seconds |
| LOW (short)      | LOW       | 0.2 seconds |
| HIGH (long)      | HIGH      | 15 minutes  |

After the long HIGH phase, the cycle repeats.

## Pin Selection

- **PIN_OUT = 23**: This pin is safe to use at boot, so it will not interfere with the ESP32 startup process.

## Usage

1. Open `Esp32Pattern.ino` in the Arduino IDE.
2. Select your ESP32 board.
3. Upload the sketch.
4. The output pattern will start on GPIO 23.

## Customization

- Change `PIN_OUT` to use a different GPIO pin (check ESP32 pinout for safe options).
- Adjust `T_HIGH_SHORT`, `T_LOW_SHORT`, and `T_HIGH_LONG` to modify durations.

## Non-blocking Operation

- The sketch is written to avoid `delay()` calls, allowing you to add other code in the `loop()` without affecting the timing.

## License

MIT