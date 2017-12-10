#include <EEPROM.h>
#include <WiFiManager.h>
#include <Button.h>
WiFiManager wifiManager;
char ssid[25];
char pwd[25];/*
char mqtt_server[25];
char mqtt_port[25];
char mqtt_user[25];
char mqtt_pwd[25];*/
const char mqtt_server[] = "m10.cloudmqtt.com";
const char mqtt_port[] = "16565";
const char mqtt_user[] = "xpskpkpr";
const char mqtt_pwd[] = "qoxaerdSjzu5";
const char AES_key[] ="2222222222222222";
const char AES_IV[] ="1111111111111111";
const char wifiMode[] = "tkip";
char configured[] = {'0', 0};

Button button = Button(D3,PULLUP);//Bring to ground



void load_data()
{
//  Serial.println("Read data from EEPROM");
  EEPROM.begin(512);
  
  int count = 0;
  int address = 0;
  char data[100] = {};
  while (count < 3)
  {
    char read_char = (char)EEPROM.read(address);
    delay(1);
    if (read_char == '#')
    {
//      Serial.println(data);
      switch (count)
      {
        case 0: strcpy(configured, data); break;
        case 1: strcpy(ssid, data); break;
        case 2: strcpy(pwd, data); break;/*
        case 3: strcpy(mqtt_server, data); break;
        case 4: strcpy(mqtt_port, data); break;
        case 5: strcpy(mqtt_user, data); break;
        case 6: strcpy(mqtt_pwd, data); break;*/
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
//  Serial.println("Read data complete");
//  Serial.println(data);
  delay(100);
}

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
  load_data();// read back in data to verify write was proper
  delay(100);
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  
}

void data_setup(char* data)
{
  char* sep = "#";
  strcat(data, configured);
  strcat(data, sep);
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
  Serial.println("Current values ready to be upated to EEPROM:");
  Serial.println(data);
  Serial.println();
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
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
      Serial.println("no networks found");
    else
    {
      Serial.print(n);
      Serial.println(" networks found");
      for (int i = 0; i < n; ++i)
       {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
        delay(10);
       }
    }
    Serial.println("");
  wifiManager.setAPCallback(configModeCallback);
  //wifiUpdate(0);
  Serial.println("Setting up Wifi");
    if (!wifiManager.autoConnect("DemoESP8266","")) {//credentials for SSID in AP mode
      
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    }
    else{
      char data[100] = {};
      Serial.println("Connected. Saving to EEPROM and resetting.");
           
      configured[0] = '1';
      data_setup(data); 
      save_data(data);  
         
      ESP.reset();
      delay(1000);
    }
    delay(1000);
  }
  
 

  




void setup() {
 // pinMode(13, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(74880);

  Serial.println();
  Serial.println();
  load_data();
  Serial.println();
  Serial.println("First read");
  Serial.print("EEPROM recorded Configured state: "); Serial.println(configured);
  Serial.print("EEPROM recorded SSID: ");Serial.println(ssid);
  Serial.print("EEPROM recorded PWD: ");Serial.println(pwd);/*
  Serial.println(mqtt_server);
  Serial.println(mqtt_port);
  Serial.println(mqtt_user);
  Serial.println(mqtt_pwd);*/

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //configured[0] ='0';//forced into AP mode for testing
  int count = 0;
  if(configured[0] != '1')//check first character in EEPROM array which is the configured status
  {
      Serial.println();
      Serial.println("Connection Time Out...");
      Serial.println("Enter AP Mode...");
      setup_wifi();
      
      Serial.println("Credentials set from user response in AP mode.");
      char data[100] = {};
      Serial.print("SSID: "); Serial.println(wifiManager.getSSID().c_str());
      Serial.print("Password: "); Serial.println(wifiManager.getPassword().c_str());   
      WiFi.begin(wifiManager.getSSID().c_str(), wifiManager.getPassword().c_str());
      Serial.println("Configuration entered. Testing connection.");
      
     
      for(int j = 0; WiFi.status() != WL_CONNECTED; j++)
      {
        Serial.print(".");
        delay(1000);

        if(j >= 100){
          Serial.println("Timeout initial connection to AP");
          configured[0] = '0';
          data_setup(data); 
          save_data(data);  
             
          ESP.reset();
          delay(1000);
        }
      }
      
    
  }
  WiFi.begin(ssid, pwd);
  Serial.println("Connected");
 
}

int buttonCount = 0;
boolean change = false;
int rst = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() == WL_CONNECTED){
    //Serial.println("Connected");//print to verify connection
    //code in this block will run only if connected
  }
  int rst = 0;

  if(button.uniquePress()){
    buttonCount++;
    change=true;
    while(button.isPressed()){
      delay(10);
      rst++;
      
      if(rst>=1000){//reset after 100 loops with button press (1 sec)
        Serial.print("EEPROM Reset Command received.");
        wifiManager.resetSettings();
        delay(10);
        Serial.print("Resetting EEPROM to default and restarting.");
        char data[100] = "0#ssid#pw123456789#x#x#x#x#x#x";
        save_data(data);
        ESP.reset();
        }
    }
  }
}
