//Example Code for AP/Client functionality for the ESP32
//Make sure to run the EEPROM sketch on a virgin board to setup the EEPROM!  EEPROM reset functions only work when outside the 
//Example extended for the ESP32 by Dr. Michael Klopfer from ESP8266 example developed by Dr. Michael Klopfer and Paolo Caraos (UC Irvine / Calit2)

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

//Watchdog Timer for ESP32 in Arduino
/*#include "esp_system.h" //see (https://github.com/espressif/arduino-esp32/issues/841) - Watchdog timer 

const int loopTimeCtl = 0;
hw_timer_t *timer = NULL;

void IRAM_ATTR resetModule(){
    ets_printf("reboot\n");
    esp_restart_noos();
}
*/

//Button Defs
#define EEPROMRST 2  //This is intended for GPIO2 for compatability, which is pin D4 on the NodeMCU, this is the GPIO2 on the ESP8266 ESP01 only - https://github.com/espressif/arduino-esp32/issues/425//Initialize Objects
WiFiManager wifiManager;
Button button = Button(EEPROMRST,PULLUP);  //hold pin high, low activates

//Define Compiler & Linker Defs
//#define IRAM0     __attribute__((section(".iram0.text"))) //required for ISR?  Disable if interrupts are not used

//Initialize global variables
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
IPAddress ip;                    // the IP address of your shield

//Counter Variables
int buttonCount = 0;
boolean change = false;
int rst = 0; //reset tracker

//Serve data once connected
//server.begin(); //prepare to serve data to a client when connected 


//**************Functions*****************

//Note:  Issues with attach interrupt may exist, consider using static void ICACHE_RAM_ATTR EEPROMReset()
//General and EEPROM Reset Fucntionality, called via interrupt, when this is called, this freezes everything and will exit will cause reset in some form or another
//note: this function has no real effect until the EEPROM is set, setting an EEPROM that is already reset does nothing
//Also, this code uses an EEPROM array, here is an alternative approach using JSON:  https://forum.arduino.cc/index.php?topic=527374.0 and https://randomnerdtutorials.com/wifimanager-with-esp8266-autoconnect-custom-parameter-and-manage-your-ssid-and-password/ and https://github.com/zhouhan0126/WIFIMANAGER-ESP32/blob/master/examples/AutoConnectWithFSParameters/AutoConnectWithFSParameters.ino

void EEPROMReset()  
  {
  WiFi.disconnect();//Close WiFi Connections
  delay(200);
  //noInterrupts(); //Disable further interrupt calls - Typically you want to turn this off, but this is the end of the line before a reset when an interrupt is called.
  int rst = 0;
  while(button.isPressed())
    {
      //timerWrite(timer, 0); //reset timer (feed watchdog) - Keep WDT Timer fed to prevent WDT restart
      delay(100); //This is in place to slow down the loop, it will freeze other operations!  Will either exit with a soft or hard reset   
      rst++;
      if(rst>=50) //Check to see if heald for more than 4 seconds, if so, hard reset
      {//reset after 40 loops with button press (4 seconds)
        Serial.println("EEPROM Reset (Hard or Soft) command received");
        //wifiManager.resetSettings();  //No need to use, you are just stacking deck chairs on the Titanic at this point, EEPROM will be erased
        Serial.println("Preparing to default EEPROM (Hard Reset)");
        delay(10);
        while(button.isPressed()) //inside the IF statement to check if button is still being pressed (pin pulled low)     
          {
           //Stay in this loop until the button is released.  This prevents bootup issues if the pin is kept low at reset, this is an issue for GPIO2
           //timerWrite(timer, 0); //reset timer (feed watchdog) - Keep WDT Timer fed to prevent WDT restart
          }
        Serial.println("Now Resetting EEPROM to default and restarting (Hard Reset)");
        delay (20);
        char data[100] = "0#ssid#pw123456789#x#x#x#x#x#x"; //blank String with 0 as EEPROM config value
        save_data(data);
        delay (500);
        Serial.println("EEPROM overwrite complete, restarting...");
        ESP.restart();  //Board will reset before leaving loop
        delay (2000);
       }
       else
       {} //Rounding out IF statement
      }
      Serial.print("Soft Reset command received!  Will be issued on Button release"); //should already be released, this is just a warning
      while(button.isPressed()) //inside the IF statement to check if button is still being pressed (pin pulled low)     
        {
         //Stay in this loop until the button is released.  This prevents bootup issues if the pin is kept low at reset, this is an issue for GPIO2
         //timerWrite(timer, 0); //reset timer (feed watchdog) - Keep WDT Timer fed to prevent WDT restart
        }
        Serial.println("Soft Reset command received, restarting...");
      delay (500);
        esp_restart_noos();  //alternative reset function
        ESP.restart(); ////alternative approach is using [ESP.restart();] //Button not held for 4 seconds, loop terminates early - this provides dual function for this reset, soft and hard reset depending on hold time
      delay (2000); 
}

void setup() 
{
 
  pinMode(EEPROMRST, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(EEPROMRST), EEPROMReset, FALLING); //Check for RESET command via interrupt (alternative approach)  //this can cause issues, the alternate (less elegant way is using the trigger in the loop), disable if you are getting crashes on reset.
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

  /* //WatchDogTimer Setup
    timer = timerBegin(0, 80, true); //timer 0, div 80
    timerAttachInterrupt(timer, &resetModule, true);
    timerAlarmWrite(timer, 3000000, false); //set time in us
    timerAlarmEnable(timer); //enable interrupt
*/
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
  wifiManager.setTimeout(60); //Timeout for WiFi Manager - this sets the max time the WiFi Manager will stay active - remember, the WiFi manager only runs when the EEPROM data is invalid or does not allow a connection, usually first run, and will be active until a tested conenction is OK then it will cache these
  //wifiManager.setBreakAfterConfig(true);  //exit after config instead of connecting, for testing
  //wifiManager.resetSettings();  //reset settings - for testing
  //wifiManager.wifiUpdate(0); //This is a test that can be turned on to force the update script to save credentials for testing.  Only uncomment in testing, function may be depricated
  //wifiManager.resetSettings();   //reset settings - for testing
  //wifiManager.setMinimumSignalQuality();
  
  Serial.println("Setting up Wifi");
    if (!wifiManager.autoConnect("DemoESP32","")) //credentials for SSID in AP mode (NOTE: to set custom ip for portal, an example is shown: wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0)))
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

  
/*
static void IRAM0 interruptHandler()  //Stub handler because there is only one interrupt event with one meaning.  This holds for mif more interrupts are to be used  (ISR Function)
{
  //EEPROMReset();  //Call the EEPROMReset function when interrupt is called, yes this is really long execution to be called by an interrupt routine!
}
*/


  
void loop() 
{
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
