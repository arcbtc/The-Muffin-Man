<p align="center"><img src="https://i.imgur.com/Oy3qzQV.png" ></p>

# THE MUFFIN MAN, LND node monitor

ESP32 + 3.5 Inch LCD (the one the <a href="https://github.com/rootzoll/raspiblitz">Raspiblitz</a> uses) LND node monitor. Keep your node locked down somewhere safe, but still have an easy way to check its status.

 ## Tutorial:

 [![muffinman](https://i.imgur.com/NiCGlNh.png)](https://www.youtube.com/watch?v=PAsIa6c8Zh4)
 
 ## Hardware:
* ESP32 NODE32S
* 3.5 Inch TFT LCD
* x10 male-female jumper cables


 ## Wiring Guide:

| FUNCTION  | TFT LCD | ESP32 |
| ------------- | ------------- | ------------- | 
| MISO  | 21  | 19  |
| MOSI  | 19  | 23  |
| SCK  | 23 | 18  |
| DC  | 18  | 17  |
| CS  | 24  | 05  |
| RST  | 22 | 16  |
| T_CS  | 26  | 04  |
| VCC (5V)  | 02  | 5v  |
| GND  | 14/25  | GND  |
| T_IRQ  | 11  | 34  |


<img src="https://i.imgur.com/tYgD3eo.png">
<img src="https://i.imgur.com/PLP3YBG.jpg">


## Installing arduino + libraries

Install the Arduino IDE,<br>
https://www.arduino.cc/en/Main/Software

Install the ESP32 hardware,<br>
https://github.com/espressif/arduino-esp32#installation-instructions

From "Manage Libraries" install,<br>
- <a href="https://github.com/lvgl/lv_arduino">lv_arduino</a> (use config files in "libaries")
- <a href="https://github.com/bblanchon/ArduinoJson">ArduinoJson</a>
- <a href="https://github.com/Bodmer/TFT_eSPI">TFT_eSPI</a> (use config files in "libaries")
- <a href="https://github.com/ricmoo/QRCode">QRCode</a>
<img src="https://i.imgur.com/mCfnhZN.png">

## To do
- Multinode support
- Telegram messaging on node error
