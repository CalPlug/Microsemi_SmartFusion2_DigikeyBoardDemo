This Firmware provides access to the programming features for the ESP8266 and ESP32.  The buttons on the board are now set with a timed sequence to allow programming of the ESP8266 and ESP32.

Developed by Dr. Michael Klopfer (UC Irvine / Calit2)
All original code is copyright of the regents of the University of California and released into the public domain.


NOTE: The on-board FTDI must be programmed so that Port B and Port C are presented as virtual com ports to the connected computer.  In programming, the lower com port is directly connected to the ESP32, the higher com port is connected via pin mapping in the SF2 to the ESP8266.  The Enable and reset functions are implemented in this firmware using the buttons on the demoboard.

Programming instructions:

Load the developed firmware to the SF2 Digikey board via FlashPro firmware up load or using Libero 11.8 SP2.

Usage:
To Program the ESP8266:
1) Plug in ESP8266 ESP01 Device
2) Remove jumpers for J10 and J11 (this isolates the ESP32) - step required if the ESP32 is soldered on
3) Press Button 2, wait aproximately 1/2 second.
4) Upload Code in Arduino IDE (mind com port and programmer settings!)
5) After upload is finished, press Button 1 (resets ESP8266), removed jumpers prevent ESP32 reset.
(Note, where implemented in sample code the EEPROM for AP/client ESP8266 firmwares will be reset by an extended hold of GPIO2)  Be aware of this operation!

To Program the ESP32 (Procedure 1) - [Only used if the ESP32 is soldered on]:
1) Remove ESP8266 module (This isolates this module from the programming mode)
2) Remove jumpers for J10 and J11 to allow sequence to set GPIO2 to LOW. This first run sets GPIO2 low.  You can use this feature with jumpers back on to manualally override the auto program and reset functionality of the Arduino IDE
3)Press Button 2, wait aproximately 1/2 second.
2) Replace jumpers for J10 and J11 to permit default program mode whereby the Arduino IDE will allow the board to ener program mode and reset it afterwards.  These jumpers allow the Arduino IDE to control the program and reset modes after GPIO2 is set to 0.
4) Upload Code in Arduino IDE (mind com port and programmer settings!)
5) Press Button 1 (resets ESP32) (optional in this procedure)
6) Replace ESP8266 module if desired

To Program the ESP32(Procedure 2: Alternate) - [Only used if the ESP32 is soldered on]:
1) Remove ESP8266 module (This isolates this module from the programming mode)
2) Make sure jumpers for J10 and J11 are in place for positions 1-2.  This procedure disables the Arduino Auto program feature and uses the capability of the SF2 programming firmware to control the ESP32 entering program mode.
3) Upload Code in Arduino IDE (mind com port and programmer settings!)
4) Press Button 1 (resets ESP32) (required in this procedure)


Note: When a button is pressed, the D9 LED comes on and action is happening until LED extinguishes.  Proceed to the next step when LED is off.

(Button 3 is hard reset for the SF2)

A version of this firmware has been released as a programmable flash file - press this up to the Microsemi SF2 Digikey Maker board PCB using FlashPro Software (https://www.microsemi.com/products/fpga-soc/design-resources/programming/flashpro#software) or build the project and upload with Libero SOC 11.8 SP2.  (see this directory: \toolkit\ESP_FWProg\designer\SF2_MSS_sys\export)
74880 is the baud rate for debug messages from the ESP01 bootloader


Note:  Observe upload settings for each board in Arduino.  This is important to make sure communications works even with proper connectivity.  Make sure you are uploading to the poper board - the serial port number is a quick indication of this.  See screenshots in the Documents folder

If a Firmware using GPIO2 is used, the EEPROM is reset by entering the programming mode (SW 2).  The reset ends with the device in programming moode.  To return to normal operation, perform a reset by pressing SW 1.
