This folder contains  the most recent Libero project for the Digikey maker Board with the Microsemi SmartFusion 2 FPGA.

To operate:
1) Upload the ESP8266 project on your Espressif ESP8266 (ESP-01):  https://github.com/CalPlug/Microsemi_SmartFusion2_DigikeyBoardDemo/tree/master/Espressif_ESP8266-ESP32_Applications/ESP8266/ESP8266_web_interface from the Arduino IDE.   You can do this using an external programmer or by using your Maker board as a firmware flasher. Instructions on using the Maker board as an external flasher are here (Note:  unless you pull jumpers and connect an external flashed, this workflow is required for programming the ESP32):  https://github.com/CalPlug/Microsemi_SmartFusion2_DigikeyBoardDemo/tree/master/toolkit/ESP_FWProg  NOTE: for some of the projects, FlashPro files have been exported for easy updating, but were not tested.  The tested workflow uses Libero SoC 11.8 to upload the eNVM and Flash project designs to the FPGA.  If executing using the debugger is desired, use Microsemi SoftConsole 5.1.  The SoftConsole 5.1 project (which runs on the Arm Cortex M3) is already in the eNVM part of the Libero SoC project and will auto-execute on bootup if flashed.

2) Upload the fabric and eNVM designs to the Maker board from this project, make sure ESP8266 ESP01 board is connected and programmed.

3) Restart and test.  Screen shots are available in the documentation folder to detail operation.  At this point the ESP8266 should act as a WiFi access point.  You should be able to connect to the application by joining this access point and pointing your browser to the IP of the ESP8266 [typically: 192.168.4.1] (see detailed instructions in the folder for the project).


[These instructions are a placeholder and need to be expanded]


