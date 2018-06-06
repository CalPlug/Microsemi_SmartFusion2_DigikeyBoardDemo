/*
 *  This version uses default values from the EEPROM to fill in the textboxes
 *  in the config portal
 * 
 */

#include <EEPROM.h>
#include <WiFiManager.h>
#include <Button.h>
#include <EMem.h>
#include <PubSubClient.h>
#include <Base64_MLC.h>  //the arduinobase64 library had to be modified to remove the need for the avr/progmem library
#include <AES.h>
#include <AES_config.h>

#define clip(x, min, max)      ((x) > (max))? (max) : ((x) < (min))? (min) : (x)

#define DEFAULT_VALUES_FROM_EEPROM//if this is defined, EEPROM values autofill WiFi credentials at AP configuration

String MAC_ID;

AES aes ;

const char* mqtt_server = "m10.cloudmqtt.com";
const char* mqtt_port = "17934";
const char* mqtt_user = "dkpljrty";
const char* mqtt_pwd = "ZJDsxMVKRjoR";
byte AES_key[] ="2222222222222222";
const char* AES_IV ="1111111111111111";
const char* wifiMode = "tkip";

char configured[] = {'0', 0};

unsigned long mqtt_time;
const unsigned long SECONDS = 1000;
const unsigned long PERIOD = 10 * SECONDS;

WiFiManager wifiManager;
WiFiManagerParameter *mqtt_server_param;
WiFiManagerParameter *mqtt_port_param;
WiFiManagerParameter *mqtt_user_param;
WiFiManagerParameter *mqtt_pwd_param;
bool validMQTTCredentials = false;

WiFiClient espClient;
PubSubClient *client;
String MQTT_TOPIC_PUB = "out/devices/"; //append MAC_ID
String MQTT_TOPIC_SUB = "in/devices/"; //append MAC_ID

int buttonCount = 0;
boolean change = false;
int rst = 0;

EMem emem;

Button button = Button(D4, PULLUP); //Bring to ground, reset button

void configModeCallback (WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
}

String encodeMessage(byte key[], String msg)
{
  byte plain[msg.length() + 1];
  msg.getBytes(plain, msg.length() + 1);
  byte cipher [N_BLOCK] ;
  aes.set_key (key, 128) ;
  aes.encrypt (plain, cipher) ;

  aes.copy_n_bytes (plain, cipher, sizeof(plain)) ;

  char data [sizeof(cipher) + 1] = {0}; //make null character array
  for (int i = 0; i < sizeof(cipher); i++) //trasfer cipher into null character array
  {
    data[i] = char (cipher[i]); //should there be a char typecast?
  }
  int inputLen = sizeof(cipher);
  int encodedLen = base64_enc_len(inputLen);
  char encoded[encodedLen];
  base64_encode(encoded, data, inputLen);
  return String(encoded);
}

String decodeMessage(byte key[], String msg, int msgLength)
{
  char encoded[msg.length() + 1];
  msg.toCharArray(encoded, msg.length() + 1);

  int input2Len = sizeof(encoded);
  int decodedLen = base64_dec_len(encoded, input2Len);
  char decoded[decodedLen];

  base64_decode(decoded, encoded, input2Len);

  String decodedString = String(decoded);

  byte cipher[decodedString.length() + 1];
  decodedString.getBytes(cipher, decodedString.length() + 1);

  byte check[msgLength];

  aes.set_key (key, 128) ;
  aes.decrypt(cipher, check) ;

  return String((char*) check).substring(0, msgLength);
}

void callback(char* topic, byte* payload, unsigned int length)
{
  char message_buff[100] = {};
  int i = 0;
  Serial.print("Message arrived:  topic: " + String(topic));
  for (i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String message_ = String(message_buff);
  Serial.println(" >>>> MESSAGE: " + message_ + " >>>> DECODED: " + decodeMessage(AES_key, message_,(int) length));
}

unsigned long reconnect() //records how long each reconnect takes
{
  unsigned long timeElapsed = millis();
  if (!client->connected())
  {
    Serial.println("Attempting to connecting to MQTT...");
    if (client->connect(MAC_ID.c_str(), emem.getMqttUser().c_str(), emem.getMqttPwd().c_str()))
    {
      Serial.println("MQTT connected");

      //publish and subscribe to topics
      client->subscribe(String(MQTT_TOPIC_SUB + "1").c_str());
      client->subscribe(String(MQTT_TOPIC_SUB + "2").c_str());
      client->subscribe(String(MQTT_TOPIC_SUB + "3").c_str());
      client->publish(MQTT_TOPIC_PUB.c_str(), "encrypt this");

      Serial.println("Published");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client->state());
      Serial.println(" will try connecting again in a few secs");
    }
  }
  return millis() - timeElapsed;
}

void EEPROMReset()
{
  WiFi.disconnect();//Close WiFi Connections
  delay(200);
  //noInterrupts(); //Disable further interrupt calls - Typically you want to turn this off, but this is the end of the line before a reset when an interrupt is called.
  int rst = 0;
  while (button.isPressed())
  {
    ESP.wdtFeed(); //Keep WDT Timer fed to prevent WDT restart
    delay(100); //This is in place to slow down the loop, it will freeze other operations!  Will either exit with a soft or hard reset
    rst++;
    if (rst >= 50) //Check to see if heald for more than 4 seconds, if so, hard reset
    { //reset after 40 loops with button press (4 seconds)

      Serial.println("EEPROM Reset (Hard or Soft) command received");
      //wifiManager.resetSettings();  //No need to use, you are just stacking deck chairs on the Titanic at this point, EEPROM will be erased
      Serial.println("Preparing to default EEPROM (Hard Reset)");

      delay(10);
      while (button.isPressed()) //inside the IF statement to check if button is still being pressed (pin pulled low)
      {
        //Stay in this loop until the button is released.  This prevents bootup issues if the pin is kept low at reset, this is an issue for GPIO2
        ESP.wdtFeed();
      }
      Serial.println("Now Resetting EEPROM to default and restarting (Hard Reset)");

      delay (20);
      String dataStr = "3#ssid#pw123456789#";           
      dataStr +=  emem.getMqttServer() +"#" +
                  emem.getMqttPort() +"#" +
                  emem.getMqttUser() +"#" +
                  emem.getMqttPwd() +"#" ; 
      
      char data[100];
      strncpy(data, dataStr.c_str(), sizeof(dataStr.c_str()));
      emem.saveData(data);
      delay (500);

      Serial.println("EEPROM overwrite complete, restarting...");
      ESP.reset();  //Board will reset before leaving loop
      delay (2000);
    }
    else
    {} //Rounding out IF statement
  }
  Serial.print("Soft Reset command received!  Will be issued on Button release"); //should already be released, this is just a warning

  while (button.isPressed()) //inside the IF statement to check if button is still being pressed (pin pulled low)
  {
    //Stay in this loop until the button is released.  This prevents bootup issues if the pin is kept low at reset, this is an issue for GPIO2
    ESP.wdtFeed(); //Keep WDT Timer fed to prevent WDT restart
  }

  Serial.println("Soft Reset command received, restarting...");

  delay (500);
  ESP.reset(); //alternative approach is using [ESP.restart();] //Button not held for 4 seconds, loop terminates early - this provides dual function for this reset, soft and hard reset depending on hold time
  delay (2000);
}

//Setup string to save into EEPROM
void data_setup(char* data)
{
  char* sep = "#";
  strcat(data, configured);
  strcat(data, sep);
  strcat(data, wifiManager.getSSID().c_str());
  strcat(data, sep);
  strcat(data, wifiManager.getPassword().c_str());
  strcat(data, sep);
  strcat(data, mqtt_server_param->getValue());
  strcat(data, sep);
  strcat(data, mqtt_port_param->getValue());
  strcat(data, sep);
  strcat(data, mqtt_user_param->getValue());
  strcat(data, sep);
  strcat(data, mqtt_pwd_param->getValue());
  strcat(data, sep);
  Serial.println("Current values ready to be upated to EEPROM:");
  Serial.println(data);
  Serial.println();
}

//Handles different possibilites after entering WiFi credentials
void setup_wifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)//if there are no networks found, resume without wifi
  {
    Serial.println("no networks found");
    String dataStr = "0#ssid#pw123456789#";           
    dataStr +=  emem.getMqttServer() +"#" +
                emem.getMqttPort() +"#" +
                emem.getMqttUser() +"#" +
                emem.getMqttPwd() +"#" ;     
    char data[100];
    strncpy(data, dataStr.c_str(), sizeof(dataStr.c_str()));
    emem.saveData(data);
    delay (500);
    delay(1000);
  }
  else
  {
    Serial.print(n);
    Serial.println("Networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setTimeout(45); // 45 sec to timeout
  //wifiUpdate(0);
  Serial.println("Setting up Wifi");
  String accessPointName = "ESP - " + MAC_ID;
  if (!wifiManager.autoConnect(accessPointName.c_str(), "")) { //credentials for SSID in AP mode
    Serial.println("failed to connect and hit timeout");
    //Timeout: Subtract config value for every timeout
    //Once the config value hits 0, the system will resume without connecting to wifi
    char data[100] = {};
    configured[0] = emem.getConfigStatus().charAt(0) -  1;
    data_setup(data);
    emem.saveData(data);
    ESP.reset();
    delay(1000);
  }
  else {
    char data[100] = {};
    Serial.println("Connected. Saving to EEPROM and resetting.");
    configured[0] = '9';
    data_setup(data);
    emem.saveData(data);

    ESP.reset();
    delay(1000);
  }
  delay(1000);
}

void isr()
{
  Serial.println("interrupted");
}

void APModeSetup()
{
  emem.loadData();
  Serial.println();
  Serial.println("First read");
  Serial.println("EEPROM recorded Configured state: " + emem.getConfigStatus());
  Serial.println("EEPROM recorded SSID: "+ emem.getWifiSsid());
  Serial.println("EEPROM recorded PWD: "+emem.getWifiPwd());
  Serial.println ("EEPROM recorded MQTT Server: "+emem.getMqttServer());
  Serial.println("EEPROM recorded MQTT Port: "+emem.getMqttPort());
  Serial.println("EEPROM recorded MQTT User: "+emem.getMqttUser());
  Serial.println("EEPROM recorded MQTT Pwd: "+emem.getMqttPwd());

 //Initialize WiFiManager objects for MQTT credentials
#ifdef DEFAULT_VALUES_FROM_EEPROM
  mqtt_server_param = new WiFiManagerParameter("mqtt_server", "mqtt_server", emem.getMqttServer().c_str(), 40);
  mqtt_port_param = new WiFiManagerParameter("mqtt_port", "mqtt_port", emem.getMqttPort().c_str(), 40);
  mqtt_user_param = new WiFiManagerParameter("mqtt_user", "mqtt_user", emem.getMqttUser().c_str(), 40);
  mqtt_pwd_param = new WiFiManagerParameter("mqtt_pwd", "mqtt_pwd", emem.getMqttPwd().c_str(), 50);
#else
  mqtt_server_param = new WiFiManagerParameter("mqtt_server", "mqtt_server", mqtt_server, 40);
  mqtt_port_param = new WiFiManagerParameter("mqtt_port", "mqtt_port", mqtt_port, 40);
  mqtt_user_param = new WiFiManagerParameter("mqtt_user", "mqtt_user", mqtt_user, 40);
  mqtt_pwd_param = new WiFiManagerParameter("mqtt_pwd", "mqtt_pwd", mqtt_pwd, 50);
#endif
  wifiManager.addParameter(mqtt_server_param);
  wifiManager.addParameter(mqtt_port_param);
  wifiManager.addParameter(mqtt_user_param);
  wifiManager.addParameter(mqtt_pwd_param);  

  MAC_ID = WiFi.macAddress();
  MQTT_TOPIC_PUB += MAC_ID + "/";
  MQTT_TOPIC_SUB += MAC_ID + "/";

  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(emem.getWifiSsid());

  if (emem.getConfigStatus().charAt(0) <= 48) //**ASCII value of '0' is 48;
  {
    Serial.println("Too many timeouts. Resuming without Wifi.");
  }
  else if (emem.getConfigStatus() != "9") //check first character in EEPROM array which is the configured status
  {
    Serial.println();
    Serial.println("Connection Time Out...");
    Serial.println("Enter AP Mode...");
    setup_wifi();
  }
  WiFi.begin(emem.getWifiSsid().c_str(), emem.getWifiPwd().c_str());
  Serial.println("Connected");
  delay(5000);
}

void setup()
{
  Serial.begin(115200);
    
  APModeSetup();
  //pinMode(D4, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(D4), isr, CHANGE);
  Serial.println("MQTT --> " +emem.getMqttServer()+ " :: "+ emem.getMqttPort());
  char *server = new char[emem.getMqttServer().length()+1];
  memset(server, 0, sizeof(server));
  emem.getMqttServer().toCharArray(server, emem.getMqttServer().length()+1);
  client = new PubSubClient(server, atoi(emem.getMqttPort().c_str()), callback, espClient);
  mqtt_time = millis();
}

void loop() {
  unsigned long currTime = millis();
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) {
    //code in this block will run only if connected
    //do MQTT operations here periodically
    if (currTime >= mqtt_time)
    {
      unsigned long reconnectTime = clip(3 * reconnect(), 4 * SECONDS, 120 * SECONDS);
      client->loop();
      if (client->connected())
      {
        //publish
        Serial.println(String(currTime) + " : Publishing!"); 
        client->publish(String(MQTT_TOPIC_PUB + "1").c_str(), encodeMessage(AES_key, "encypt this").c_str()); 
      }
      mqtt_time = currTime + reconnectTime;
    }
  }

  if (button.uniquePress()) {
    EEPROMReset();
  }
}
