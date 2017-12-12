# Microsemi_SmartFusion2_DigikeyBoardDemo for the Maker Board
Demonstration application for the Microsemi/Digikey Demonstration board.
Project developed by Crystal Lai, Brandon Metcalf, and Paolo Caraos (UC Irvine / Calit2 Microsemi Innovation Laboratory)
Project Lead:  Crystal Lai
Project Advisors: Dr. Michael Klopfer and Prof. G.P. Li
Dec 10, 2017 v. 1.0

Project copyright of the Regents of the University of California and released into the bublic domain.  Project elements copyright of denoted authors and used fairly per licensing requirements.


THis project contains two parts:  1a) The FPGA fabric project (Built in Libero 11.8), 1b) The Cortex-M3 Microcontroller Project (Built in SoftConsole 5.1), and 2) The ESP8266 program built in Arduino IDE with the plugin for Espressif ESP8266 programming.  This module provides WiFi connectivity.  Programming capability can be added on to the Arduino IDE (v. 1.8 or higher based on required libraries) (https://github.com/esp8266/Arduino) and is installed via the Boards Manager.  A quick guide to installing this was developed by Sparkfun:  https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/installing-the-esp8266-arduino-addon.  Sparkfun also developed an analogus guide for the Espressif ESP32:  https://learn.sparkfun.com/tutorials/esp32-thing-hookup-guide/installing-the-esp32-arduino-core

Note:  The Microsemi SmartFusion Digikey Maker Board has the capability to interface both the ESP8266 (8 pin module) in addition to a ESP32 module that is soldered down to the PCB as an optional addition.

Other than the ones included in this project, the additional libraries referenced are part of the following ESP8266 board package for the Arduino IDE: https://github.com/esp8266/Arduino/.  All required libraries except those included with the project will be installed when the board programming capability is setup.

Note:  The Device "M2S010-TQ144" should target the device on the board in Libero, the G submodel denotes ROHS compliance.
