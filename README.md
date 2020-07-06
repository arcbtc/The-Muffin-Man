<p align="center"><img src="https://i.imgur.com/cs8LXEu.png" width="400px"></p>

# THE MUFFIN MAN, LND node monitor

ESP32 + 3.5 Inch LCD (the one used for the <a href="https://github.com/rootzoll/raspiblitz">Raspiblitz</a>) LND node monitor. Keep your node locked down somewhere safe, but still have an easy way to check its status.

 # Hardware: (USE THE TFT_eSPI LIBRARY IN THIS FOLDER FOR THE CORRECT DRIVERS!!!)
* ESP32 NODE32S
* 1.8 TFT 160/128
* 4x4 adhesive matrix keypad
* x8 female-female jumper cables

[![ESP32 GPIO Map](https://i.imgur.com/PLP3YBG.jpg)]

* ESP32 DevKit V1

[![ESP32s GPIO Map](https://i.imgur.com/iK3pCjt.jpg)]


# Installing arduino + libraries

Install the Arduino IDE,<br>
https://www.arduino.cc/en/Main/Software

Install the ESP32 hardware,<br>
https://github.com/espressif/arduino-esp32#installation-instructions

From "Manage Libraries" install,<br>
- Keypad
- ArduinoJson
- TFT_eSPI
- QRCode
![a](https://i.imgur.com/mCfnhZN.png)

# To do
Multinode support
Telegram message
