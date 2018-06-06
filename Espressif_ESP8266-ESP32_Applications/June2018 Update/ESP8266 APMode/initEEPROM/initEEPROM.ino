#include <EMem.h>
EMem emem;
void setup() {
  // put your setup code here, to run once:
  char *data = "3#CalPlugSimHome#A8E61C58F8#m10.cloudmqtt.com#17934#dkpljrty#ZJDsxMVKRjoR#";
  emem.saveData(data);
  emem.loadData();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.println("EEPROM recorded Configured state: " + emem.getConfigStatus());
  Serial.println("EEPROM recorded SSID: " + emem.getWifiSsid());
  Serial.println("EEPROM recorded PWD: " + emem.getWifiPwd());
  Serial.println ("EEPROM recorded MQTT Server: " + emem.getMqttServer());
  Serial.println("EEPROM recorded MQTT Port: " + emem.getMqttPort());
  Serial.println("EEPROM recorded MQTT User: " + emem.getMqttUser());
  Serial.println("EEPROM recorded MQTT Pwd: " + emem.getMqttPwd());
  delay(500);
}
