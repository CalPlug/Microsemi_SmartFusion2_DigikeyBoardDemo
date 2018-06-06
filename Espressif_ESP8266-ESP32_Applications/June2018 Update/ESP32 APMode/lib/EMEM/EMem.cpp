#include "EMem.h"

EMem::EMem() {DEBUG_PRINT("EMem object created.");}

template <typename Generic>
void EMem::DEBUG_PRINT(Generic txt)
{
  if (_DEBUG)
  {
    Serial.print("*EMEM: ");
    Serial.println(txt);
  }
}

void EMem::saveData(char* data) 
{
	DEBUG_PRINT("Write data to EEPROM");
	EEPROM.begin(512);
	for (int i = 0; i < strlen(data); ++i) 
	{
		EEPROM.write(i, (int)data[i]);
		delay(1);
	}
	EEPROM.commit();
	DEBUG_PRINT("Write data complete");
	loadData();
	delay(100);
}

void EMem::wipeData()
{
  DEBUG_PRINT("Wipe EEPROM");
  char* sep = "#";
  EEPROM.begin(512);
  for (int i = 0; i < 6; ++i)
  {
    EEPROM.write(i, (int)sep);
    delayMicroseconds(1000);
    EEPROM.write(i, 50);
    delayMicroseconds(1000);
  }
  EEPROM.commit();
  DEBUG_PRINT("Wipe data complete");
}

void EMem::loadData() 
{
  DEBUG_PRINT("Call to read data from EEPROM");
  EEPROM.begin(512);
  int count = 0;
  int address = 0;
  char data[100] = {};
  while (count < 7)
  {
    char read_char = (char)EEPROM.read(address);
    delay(1);
    if (read_char == '#')
    {
      DEBUG_PRINT(data);
      switch (count)
      {
        case 0: _configured = data; break;
        case 1: _wifiSsid = data; break;
        case 2: _wifiPwd = data; break;
        case 3: _mqttServer = data; break;
        case 4: _mqttPort = data; break;
        case 5: _mqttUser = data; break;
        case 6: _mqttPwd =  data; break;
      }
      count++;
      strcpy(data,"");
    } 
    else
    {
      strncat(data, &read_char, 1);  
    }
    ++address;
  }
  delay(100);
  DEBUG_PRINT("<--Read data complete, this was read");
}

void EMem::updateConfigStatus(String s) {
  _configured = s;
}

void EMem::updateWiFiSsid(String s){
  _wifiSsid = s;
}

void EMem::updateWiFiPwd(String s){
  _wifiPwd = s;
}

String EMem::getConfigStatus() {
  return _configured;
}

String EMem::getWifiSsid() {
	return _wifiSsid;
}

String EMem::getWifiPwd() {
	return _wifiPwd;
}

String EMem::getMqttServer() {
	return _mqttServer;
}

String EMem::getMqttPort() {
	return _mqttPort;
}

String EMem::getMqttUser() {
	return _mqttUser;
}

String EMem::getMqttPwd() {
	return _mqttPwd;
}