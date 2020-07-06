<p align="center"><img src="https://i.imgur.com/cs8LXEu.png" width="400px"></p>

# THE MUFFIN MAN, LND node monitor

ESP32 + 3.5 Inch LCD (the one used for the <a href="https://github.com/rootzoll/raspiblitz">Raspiblitz</a>) LND node monitor. Keep your node locked down somewhere safe, but still have an easy way to check its status.

 # Hardware:
* ESP32 NODE32S
* 3.5 Inch TFT LCD
* x10 male-female jumper cables


 # Wiring Guide:

| FUNCTION  | TFT LCD | ESP32 |
| ------------- | ------------- | ------------- | 
| MISO  | 21  | 19  |
| MOSI  | 19  | 23  |
| SCK  | 23 | 18  |
| DC  | 18  | 02  |
| CS  | 24  | 05  |
| RST  | 22 | 19  |
| T_CS  | 26  | 04  |
| VCC (5V)  | 02  | 5v  |
| GND  | 14/25  | GND  |
| T_IRQ  | 11  | 34  |

![ESP32s GPIO Map](https://i.imgur.com/tYgD3eo.png)
![ESP32s GPIO Map](https://i.imgur.com/PLP3YBG.jpg)


# Installing arduino + libraries

Install the Arduino IDE,<br>
https://www.arduino.cc/en/Main/Software

Install the ESP32 hardware,<br>
https://github.com/espressif/arduino-esp32#installation-instructions

From "Manage Libraries" install,<br>
- <a href="https://github.com/lvgl/lv_arduino">lv_arduino</a> 
- <a href="https://github.com/bblanchon/ArduinoJson">ArduinoJson</a>
- <a href="https://github.com/Bodmer/TFT_eSPI">TFT_eSPI</a>
- <a href="https://github.com/ricmoo/QRCode">QRCode</a>
[a](https://i.imgur.com/mCfnhZN.png)

# To do
Multinode support
Telegram messaging on node error
