#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <EMem.h>
#include <WiFiManager.h>

EMem emem;
WiFiManager wifiManager;

void configModeCallback (WiFiManager *myWiFiManager) 
{
  #ifdef DEBUG_WIFI
  Serial.println("Entered config mode");
  #endif
}

void resetWifi() 
{
  wifiManager.resetSettings();
}
void setup_wifi() 
{
  wifiManager.setAPCallback(configModeCallback);

  #ifdef DEBUG_WIFI
  Serial.println("Setting up Wifi");
  #endif

  if (!wifiManager.autoConnect("Water Cooler Buddy DeviceConfig", "watercoolerbuddy")) 
  {
    #ifdef DEBUG_WIFI
    Serial.println("failed to connect and hit timeout");
    #endif
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
  }
}


void data_setup(char* data) 
{
  char* sep = "#";
  strcat(data, wifiManager.getSSID().c_str());
  strcat(data, sep);
  strcat(data, wifiManager.getPassword().c_str());
  strcat(data, sep);
  
  #ifdef DEBUG
  Serial.println("This is the final string:");
  Serial.println(data);
  Serial.println();
  #endif
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  emem.loadData();
  WiFi.begin(emem.getWifiSsid().c_str(), emem.getWifiPwd().c_str());

  #ifdef DEBUG
  Serial.println(emem.getWifiSsid());
  Serial.println(emem.getWifiPwd());
  Serial.println(emem.getMqttServer());
  Serial.println(emem.getMqttPort());
  Serial.println(emem.getMqttUser());

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(emem.getWifiSsid());
  #endif

  for (int c = 0; c <= 30 and WiFi.status() != WL_CONNECTED; ++c) 
  {
    delay(500);

    Serial.print(".");
  
    if (c == 30) 
    {
     
      #ifdef DEBUG
      Serial.println();
      Serial.println("Connection Time Out...");
      Serial.println("Enter AP Mode...");
      #endif

      setup_wifi();
      char data[100] = "";
      data_setup(data);
      emem.saveData(data);

      for (int i = 0; i < 3; ++i)
      {
        Serial.print(".");
      }
    
      ESP.reset();
    }
  }
  #ifdef DEBUG
  Serial.println("Connected.");
  #endif
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
