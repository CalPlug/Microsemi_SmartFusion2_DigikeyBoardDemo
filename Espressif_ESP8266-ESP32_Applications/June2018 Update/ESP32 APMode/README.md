How to use AP mode
=====

*NOTE: EEPROM must first be initialized with the program initEEPROM.ino* You must 
first upload this program to initialize the values in the EEPROM. These values are
necessary for the AP mode control flow to work.

The purpose of this AP mode script is to provide the control flow for saving
credentials obtained via WiFiManager library to the EEPROM, since the WiFiManager
library does not provide that feature. There are 3 key functions in this control flow.

* APModeSetup() is used to setup the ap configuration portal that will
gather WiFi and MQTT credentials. In the standard arduino program, this 
function belongs in the setup().

* setup_wifi() is contained within the APModeSetup function which handles
the 3 cases that can happen after credentials are entered. It handles
what is saved into the EEPROM. There's no need to call this function outside
of APModeSetup().

* EEPROMReset() is the function that should be called when an hardware interface
such as a button is pressed. This function lets the user reset the hardware
or clear the saved credentials within the EEPROM. Note: This does not clear
the MQTT credentials. Only the WiFi credentials is cleared.

*Please use APMode_w_MQTT_AES_EBC.ino for the most updated reference*

There are currently independent WiFiManager libraries that have EEPROM
saving features. However, the quality of those libraries are not the best.
In the future, there may be an implementation of WiFiManager that has this feature.
