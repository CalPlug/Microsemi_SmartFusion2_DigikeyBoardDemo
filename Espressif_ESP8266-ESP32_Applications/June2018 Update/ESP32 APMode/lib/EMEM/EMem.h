#ifndef EMem_h
#define EMem_h

#include <EEPROM.h>
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class EMem {
public:
	EMem();
	void saveData(char* data);// only used for clearing EEPROM data
	void loadData();
	void wipeData();

	void updateConfigStatus(String s);
	void updateWiFiSsid(String s);
	void updateWiFiPwd(String s);

	String getConfigStatus();
	String getWifiSsid();
	String getWifiPwd();
	String getMqttServer();
	String getMqttPort();
	String getMqttUser();
	String getMqttPwd();

private:

	template <typename Generic>
	void DEBUG_PRINT(Generic txt);

	String _configured;
	String _wifiSsid;
	String _wifiPwd;
	String _mqttServer;
	String _mqttPort;
	String _mqttUser;
	String _mqttPwd;

	boolean _DEBUG = false;
};


#endif
