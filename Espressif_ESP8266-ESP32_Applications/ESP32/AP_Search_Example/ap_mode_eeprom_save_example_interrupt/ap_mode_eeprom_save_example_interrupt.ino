                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     //Example Code for AP/Client functionality for the ESP32
//Make sure to run the EEPROM sketch on a virgin board to setup the EEPROM!  EEPROM reset functions only work when outside the 
//Example extended for the ESP32 by Dr. Michael Klopfer from ESP8266 example developed by Dr. Michael Klopfer and Paolo Caraos (UC Irvine / Calit2)
//Ground GPIO2 on the ESP32 for 3 seconds to perform a soft reset.  Push the button 13 times in a row faster than 1 press for 1.5 seconds within a 30 second window to reset credentials

//this is a quick demo of functionality - it does not include structure within the RTOS framework!!

//ESP32 example Wifi Connectivity
#include <Button.h> //Debounce functionality, library tested on ESP8266, functional on ESP32
#include <WiFi.h> //Default ESP32 Arduino Wifi Library
#include <EEPROM.h>  ////handles save and read with EEPROM on ESP32, use ESP32 version included with default Arduino core for ESP32
#include <WiFiManager.h> //Updated ESP32 WifiManager!  Use the version included with this code!
#include <WiFiClient.h>
#include <DNSServer.h> //used to handle the captive portal functionality (functional alone) if implemented (https://github.com/zhouhan0126/DNSServer---esp32)
#include <ESPmDNS.h>

//Simple Setup to serve data on HTTP when requested on port 80
#include <WebServer.h> //Modified for the ESP32 (https://github.com/bbx10/WebServer_tng)
//WiFiServer server(80);


//Button Defs
#define EEPROMRST 2  //This is intended for GPIO2 for compatability, which is pin D4 on the NodeMCU, this is the GPIO2 on the ESP8266 ESP01 only - https://github.com/espressif/arduino-esp32/issues/425//Initialize Objects
Button button = Button(EEPROMRST,PULLUP);  //hold pin high, low activates
WiFiManager wifiManager;
volatile int buttoninterruptCounter = 0;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

//Initialize global variables
char ssid[25];
char pwd[25];
const char mqtt_server[] = "m10.cloudmqtt.com";
const char mqtt_port[] = "16565";
const char mqtt_user[] = "xpskpkpr";
const char mqtt_pwd[] = "qoxaerdSjzu5";
const char AES_key[] ="2222222222222222";
const char AES_IV[] ="1111111111111111";
const char wifiMode[] = "tkip";
char configured[] = {'0', 0};
IPAddress ip;                    // the IP address of your shield

//Counter Variables
int buttonCount = 0;
boolean change = false;
int rst = 0; //reset tracker
long int timetracker = 0;
boolean eepromresetflag = 0; //command to reset EEPROM

//Serve data once connected
//server.begin(); //prepare to serve data to a client when connected 


//**************Functions*****************

//Note:  Issues with attach interrupt may exist, consider using static void ICACHE_RAM_ATTR EEPROMReset()
//General and EEPROM Reset Fucntionality, called via interrupt, when this is called, this freezes everything and will exit will cause reset in some form or another
//note: this function has no real effect until the EEPROM is set, setting an EEPROM that is already reset does nothing
//Also, this code uses an EEPROM array, here is an alternative approach using JSON:  https://forum.arduino.cc/index.php?topic=527374.0 and https://randomnerdtutorials.com/wifimanager-with-esp8266-autoconnect-custom-parameter-and-manage-your-ssid-and-password/ and https://github.com/zhouhan0126/WIFIMANAGER-ESP32/blob/master/examples/AutoConnectWithFSParameters/AutoConnectWithFSParameters.ino

void IRAM_ATTR handleInterrupt() 
{  //long for an ISR, but works
  portENTER_CRITICAL_ISR(&mux);
  long int temptimeholder = millis();
  long int timedifference;
  timedifference = temptimeholder-timetracker;
 
    
  if (timedifference < 0 || timedifference > 10000) //assume a rollover happened or a period of greater than 10 seconds has elapsed between button pushes, in either case, count this as the first button press in the series
  {
    buttoninterruptCounter=0,//reset interrupt counter
    buttoninterruptCounter++; //count in default case when triggered for a whole new set
    timetracker = temptimeholder;
  }
  
 if (timedifference > 80 && timedifference < 1500) //ignore presses less than 80ms and greater than 1.5 seconds spacing
 {
    buttoninterruptCounter++;
    timetracker = temptimeholder;
 }
 
 if (buttoninterruptCounter > 12)//reset on 13 presses of the button
  {
     eepromresetflag= 1;  //set command to reset!
     Serial.println("EEPROM RESET INITIATED"); //remove this flag in production
  } 

  portEXIT_CRITICAL_ISR(&mux);
}

 
void EEPROMReset()  
  {
  WiFi.disconnect();//Close WiFi Connections
  delay(100);
  //portEXIT_CRITICAL_ISR(&mux); // Consider disable of further interrupt calls - Typically you want to turn this off, but this is the end of the line before a reset when an interrupt is called.
  //wifiManager.resetSettings();  //No need to use - just stacking deck chairs on the Titanic at this point - EEPROM will be erased
  Serial.println("Preparing to default EEPROM (Hard Reset)");
  Serial.println("Now Resetting EEPROM to default and restarting (Hard Reset)");
  char data[100] = "0#ssid#pw123456789#x#x#x#x#x#x"; //blank String with 0 as EEPROM config value
        save_data(data);
        delay (100);
        Serial.println("EEPROM overwrite complete, restarting...");
        ESP.restart();  //Board will reset before leaving loop
        delay (500);
        esp_restart_noos();  //alternative reset function
        ESP.restart(); ////alternative approach is using [ESP.restart();] //Button not held for 4 seconds, loop terminates early - this provides dual function for this reset, soft and hard reset depending on hold time
      delay (2000); //Should never hit this point
      Serial.println("You should never see me!");
}
     

void setup() 
{
  pinMode(EEPROMRST, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EEPROMRST), handleInterrupt, RISING); //catches interrupt on the buutton release on the rising edge - Check for RESET command via interrupt 
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  load_data(); //Load Data From EEPROM
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
        if(j >= 100)
        {
          Serial.println("Timeout initial connection to AP, credentials invalid?");
          configured[0] = '0';
          data_setup(data); 
          save_data(data);  
          ESP.restart();
          delay(1000);
        }
      }
  }
  WiFi.begin(ssid, pwd);
  Serial.println("Connected");
 }

void load_data()
{
  Serial.println("Call to read data from EEPROM");
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
      Serial.println(data);
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
  delay(100);
  Serial.println("<--Read data complete, this was read");
}

void save_data(char* data)
{
  Serial.println("Call to write data to EEPROM");
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
  Serial.println(WiFi.softAPIP());
  // print the ssid that we should connect to to configure the ESP32
  Serial.print("Created config portal AP named:  ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //more info on the captive portal:  https://diyprojects.io/wifimanager-library-easily-manage-wi-fi-connection-projects-esp8266/#.WrviTYj4_cs
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
  
  Serial.print("Current values ready to be upated to EEPROM: ");
  Serial.println(data);
  Serial.println();
}

void setup_wifi() 
{ 
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
        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN )?" ":"*"); //ENC_TYPE_NONE is for ESP8266 and is assumed to be a byte = 7? Note:  TKIP (WPA) = 2, WEP = 5, CCMP (WPA) = 4, NONE = 7, AUTO = 8
        delay(10);
       }
    }
  Serial.println("");
  wifiManager.setAPCallback(configModeCallback);  //fetches ssid and pass and tries to connect
  wifiManager.setTimeout(120); //Timeout for WiFi Manager - this sets the max time the WiFi Manager will stay active - remember, the WiFi manager only runs when the EEPROM data is invalid or does not allow a connection, usually first run, and will be active until a tested conenction is OK then it will cache these
  //wifiManager.setBreakAfterConfig(true);  //exit after config instead of connecting, for testing
  //wifiManager.resetSettings();  //reset settings - for testing
  //wifiManager.wifiUpdate(0); //This is a test that can be turned on to force the update script to save credentials for testing.  Only uncomment in testing, function may be depricated
  //wifiManager.resetSettings();   //reset settings - for testing
  //wifiManager.setMinimumSignalQuality();
  
  Serial.println("Setting up Wifi");
    if (!wifiManager.autoConnect("ESP32Setup","")) //credentials for SSID in AP mode (NOTE: to set custom ip for portal, an example is shown: wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0)))
    {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep in another use application
    ESP.restart();
    }
    else
    {
      char data[100] = {};
      Serial.println("Connected. Saving to EEPROM and resetting.");
      configured[0] = '1';
      data_setup(data); 
      save_data(data);  
      ESP.restart();
      delay(1000);
    }
    delay(1000);
  }

   
void loop() 
{
   if (eepromresetflag != 0)//reset on 13 presses of the button
  {
     EEPROMReset();  //set command to reset!
  } 
int loopreset=0;
    while(button.isPressed())
    {
      delay(80);  //hold in delay until WDT causes a reset or the loop is entered
      //pet watchdog to 
      loopreset++;
      if (loopreset>15)
      {
        Serial.println("Button Initiated Soft Reset"); //this should reset in 3 seconds of button press (soft reset)
        ESP.restart();
      }
    }
   //Code that operates whether or not WiFi is connected, but only once the main loop is reached!
    Serial.println("In Loop: ");
    //CODE
    //CODE
  /* Example to serve data once connected
    if (button.uniquePress()) //alternative to call reset function from interrupt - a better way may be to count button presses rather than relying on the WDT!
      {
            EEPROMReset();
            //Comment: Final Developer should provide user a means to know reset was triggered
      }
      */
 
  if(WiFi.status() == WL_CONNECTED)
    {   
    /*
    //code in this block will run only if connected
      WiFiClient client = server.available();   // Listen for incoming clients (see this example on simple HTTP servers: https://randomnerdtutorials.com/wifimanager-with-esp8266-autoconnect-custom-parameter-and-manage-your-ssid-and-password/)

      if (client) {                             // If a new client connects,
    Serial.println("New client connection");          // print a message out in the serial port
     //CODE
      }
     */
    //CODE
    
    Serial.print("I'm Connected! My IP is: ");
     //print the local IP address
    ip = WiFi.localIP();
    Serial.println(ip);
    Serial.printf(" with MAC address: %s\n", WiFi.macAddress().c_str());
    Serial.printf(" with BSSID: %s\n", WiFi.BSSIDstr().c_str());
    Serial.printf(" with a RSSI (signal) of: %d dBm\n", WiFi.RSSI());
    
    }
    delay (1000); //this is just temporary to slow down the loop in testing
  }
