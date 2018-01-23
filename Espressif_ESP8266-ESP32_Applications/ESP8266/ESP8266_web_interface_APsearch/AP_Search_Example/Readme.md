AP Search for ESP8266.

This code uses the EEPROM to store credentials for the saved WiFi credentials.  On bootup they are cached and available for use.  If the EEPROM is reset (using a long button push) the device will reenter AP mode on reset and display a Site Survey to allow the user to pick out a new AP to join.  Once the join is complete, the device will reset and start operating as a client device connected to the WiFi network the credentials are set for.

This is the baseis for WiFi management for a number of applications.
