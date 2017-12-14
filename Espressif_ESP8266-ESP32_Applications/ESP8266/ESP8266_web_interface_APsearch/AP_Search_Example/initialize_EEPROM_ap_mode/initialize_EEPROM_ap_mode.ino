#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#define NUM_ELEMENTS 10
char data2[100];

void save_data(char* data)
{
  Serial.println("Write data to EEPROM");
  EEPROM.begin(512);
  for (int i = 0; i < strlen(data); ++i)
  {
    EEPROM.write(i, (int)data[i]);
    delay(1);
  }
  EEPROM.commit();
  Serial.println("Write data complete");
  delay(100);
}

void load_data(char* data)
{
  Serial.println("Read data from EEPROM");
  EEPROM.begin(512);
  int count = 0;
  int address = 0;
  while (count < NUM_ELEMENTS)
  {
    char read_char = (char)EEPROM.read(address);
    delay(1);
    strncat(data, &read_char, 1);
    if (read_char == '#')
      ++count;
    ++address;
  }
  Serial.println("Read data complete");
  Serial.println(data);
  delay(100);
}

void wipe_data() 
{
  Serial.println("Wipe EEPROM");
  char* sep = "#";
  EEPROM.begin(512);
  for (int i = 0; i < NUM_ELEMENTS; ++i)
  {
    EEPROM.write(i, (int)sep);
    delay(1);
    EEPROM.write(i, 50);
    delay(1);
  }
  EEPROM.commit();
  Serial.println("Wipe data complete");
}

void setup() {
  Serial.begin(74880);
  Serial.println();
  Serial.println();
  Serial.println("HELLO PROGRAM");
  const char configured[] = "0";
  const char ssid[] = "Binh";
  const char pwd[] = "12345678";
  const char mqtt_server[] = "m10.cloudmqtt.com";
  const char mqtt_port[] = "16565";
  const char mqtt_user[] = "xpskpkpr";
  const char mqtt_pwd[] = "qoxaerdSjzu5";
  const char AES_key[] ="2222222222222222";
  const char AES_IV[] ="1111111111111111";
  const char wifiMode[] = "tkip";

  wipe_data();
  char* sep = "#";
  char data[100] = {};
  strcat(data, configured);
  strcat(data, sep);
  strcat(data, ssid);
  strcat(data, sep);
  strcat(data, pwd);
  strcat(data, sep);
  strcat(data, mqtt_server);
  strcat(data, sep);
  strcat(data, mqtt_port);
  strcat(data, sep);
  strcat(data, mqtt_user);
  strcat(data, sep);
  strcat(data, mqtt_pwd);
  strcat(data, sep);
  strcat(data, AES_key);
  strcat(data, sep);
  strcat(data, AES_IV);
  strcat(data, sep);
  strcat(data, wifiMode);
  strcat(data, sep);
  
  Serial.println("This is the final string:");
  Serial.println(data);
  Serial.println();

//  Serial.println("First read:");
//  char data3[100];
//  load_data(data3);

  save_data(data);

  Serial.println("Second read:");
  
  load_data(data2);
  Serial.println(data2);

//  wipe_data();
//  Serial.println("Second read:");
//  char data4[100];
//  load_data(data4);
}


void loop() {
  // put your main code here, to run repeatedly:

}
