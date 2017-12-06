#include <ESP8266WebInterface.h>

// Replace with your network credentials
const char* ssid = "UCInet Mobile Access";
const char* password = "";
const char* AP_PW = "CalPlug";
ESP8266WebServer server(80);
WebInterface interface;

void setupWiFi()
{

  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP8266 - " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, AP_PW);
}


int RESET = 0;
void setup(void)
{
  delay(1000);
  Serial.begin(74880);/*Baude rate used to match ESP8266 firmware messages, so baud is consistent*/
  WiFi.begin(ssid, password);
  
  setupWiFi();

  // Wait for connection
  int i;
  for ( i = 0; i <= 30 && WiFi.status() != WL_CONNECTED; i++) 
  {
    delay(500);
    Serial.print(".");
   
  }
  
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("");
    Serial.println("Timeout! Failed to connect.");
    Serial.println("Connect to 192.168.4.1");
  }else{    
    Serial.println("");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  
  server.on("/", [](){
    server.send(200, "text/html", interface.getHomePage(true));
  });
  server.on("/LED0_ON", [](){
    server.send(200, "text/html", interface.getHomePage(false));
    Serial.println("*L0O");
    delay(1000);
  });
  server.on("/LED0_OFF", [](){
    server.send(200, "text/html", interface.getHomePage(false));
    Serial.println("*L0F");
    delay(1000); 
  });
  server.on("/LED1_ON", [](){
    server.send(200, "text/html", interface.getHomePage(false));
    Serial.println("*L1O");
    delay(1000);
  });
  server.on("/LED1_OFF", [](){
    server.send(200, "text/html", interface.getHomePage(false));
    Serial.println("*L1F");
    delay(1000); 
  });
  
   server.on("/RESET", [](){
    server.send(200, "text/html", interface.getHomePage(false));
    Serial.println("*ESP_RST");   
    delay(1000);//delay long enough for the browser to refresh before reset 
    RESET = 1;    
  });
  server.begin();
  Serial.println("HTTP server started");
}

int rst_count = 0;
void loop(void){
  server.handleClient();
  
  if(Serial.available()){
    char s[BUFFER_SIZE] = {0};
    interface.newMessage("");
    for(int i = 0; Serial.available(); i++) {
       s[i] = Serial.read();
    }
    interface.appendMessage(String(s));
    Serial.println(String(s));
  }
  if(RESET == 1){
    rst_count++;
    if(rst_count == 15){//must cycle a few more times to go to home page before resetting ESP8266
      ESP.reset(); 
    }
  }  
} 
