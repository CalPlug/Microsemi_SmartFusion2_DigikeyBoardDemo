#include <EEPROM.h>
#include <WiFiManager.h>
WiFiManager wifiManager;
char ssid[25];
char pwd[25];
char mqtt_server[25];
char mqtt_port[25];
char mqtt_user[25];
char mqtt_pwd[25];

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

void load_data()
{
//  Serial.println("Read data from EEPROM");
  EEPROM.begin(512);
  int count = 0;
  int address = 0;
  char data[100];
  char temp_encryption_key[25];
  while (count < 7)
  {
    char read_char = (char)EEPROM.read(address);
    delay(1);
    if (read_char == '#')
    {
      ++count;
//      Serial.println(data);
      switch (count)
      {
        case 1: strcpy(ssid, data); break;
        case 2: strcpy(pwd, data); break;/*
        case 3: strcpy(mqtt_server, data); break;
        case 4: strcpy(mqtt_port, data); break;
        case 5: strcpy(mqtt_user, data); break;
        case 6: strcpy(mqtt_pwd, data); break;*/
      }
      strcpy(data,"");
    } 
    else
    {
      strncat(data, &read_char, 1);  
    }
    ++address;
  }
//  Serial.println("Read data complete");
//  Serial.println(data);
  delay(100);
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  
}

void setup_wifi() { 

/*  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());*/

  wifiManager.setAPCallback(configModeCallback);
  //wifiUpdate(0);
  Serial.println("Setting up Wifi");
    if (!wifiManager.autoConnect("CalPlug ESP8266","CalPlug")) {
      
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
  
 

  
}

void data_setup(char* data)
{
  char* sep = "#";
  strcat(data, wifiManager.getSSID().c_str());
  strcat(data, sep);
  strcat(data, wifiManager.getPassword().c_str());
  strcat(data, sep);
  /*
  strcat(data, wifiManager.MQTT_server.c_str());
  strcat(data, sep);
  strcat(data, wifiManager.MQTT_port.c_str());
  strcat(data, sep);
  strcat(data, wifiManager.MQTT_user.c_str());
  strcat(data, sep);
  strcat(data, wifiManager.MQTT_pass.c_str());
  strcat(data, sep);
  strcat(data, wifiManager.Encryption_Key.c_str());
  strcat(data, sep); */
  Serial.println("This is the final string:");
  Serial.println(data);
  Serial.println();
}

void setup() {
 // pinMode(13, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  load_data();
  Serial.println();
  Serial.println("First read");
  Serial.println(ssid);
  Serial.println(pwd);
  Serial.println(mqtt_server);
  Serial.println(mqtt_port);
  Serial.println(mqtt_user);
  Serial.println(mqtt_pwd);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pwd);

  int count = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    count++;
    if (count == 30)
    {
      Serial.println();
      Serial.println("Connection Time Out...");
      Serial.println("Enter AP Mode...");
      setup_wifi();
      char data[100];
      data_setup(data);
      save_data(data);
      ESP.reset();
      delay(1000);
    }
  }
  Serial.println("connected");
 
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() == WL_CONNECTED)
    Serial.println();
}
