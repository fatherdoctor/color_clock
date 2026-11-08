# color_clock

An Arduino clock that shows the time on a four-digit display and as two rings of colored lights, with a pink-and-cyan look by day and a dimmer, multicolor look at night.

## Features

- 12-hour time on a TM1637 seven-segment display.
- A 40-pixel NeoPixel ring: 24 pixels mark the hour, 16 mark the minute. The current hour and minute pixels fade in and out.
- Day mode (05:00 to 17:59): pink hour ring, cyan minute ring, brightness 160 of 255.
- Night mode (all other hours): the hour ring is a color map from red through yellow, green, blue and purple, and the minute ring takes the color of the current hour, at brightness 80 of 255.
- A DS3231 real-time clock keeps time while unpowered. If it has lost power, the sketch sets it to the time the sketch was compiled.

## Hardware

| Part | Arduino pin |
| --- | --- |
| TM1637 seven-segment display | CLK 5, DIO 3 |
| NeoPixel ring, 40 pixels (GRB, 800 kHz) | 7 |
| DS3231 real-time clock | I2C (SDA, SCL for your board) |

## Installation

1. In the Arduino IDE, install these libraries from the Library Manager: RTClib, Adafruit NeoPixel, and SevenSegmentTM1637 (it provides `SevenSegmentExtended`).
2. Wire the parts as in the table above.
3. Open `Arduino Code/color_clock.ino`, choose your board and port, and upload.

To change the pixel counts, pins or the two brightness levels, edit the constants at the top of the sketch. The serial monitor (9600 baud) reports if the clock module is not found.

## Housing

`Clock Housing/front_back_plate_template.SLDPRT` is a SolidWorks template for the front and back plate.

## License

MIT. See [LICENSE](LICENSE).
