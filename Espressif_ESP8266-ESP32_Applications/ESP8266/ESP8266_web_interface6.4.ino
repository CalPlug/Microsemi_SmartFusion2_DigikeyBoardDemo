#include <ESP8266WebInterface.h>

// Replace with your network credentials
const char* ssid = "UCInet Mobile Access";
const char* password = "";
const char* AP_PW = "CalPlug";
ESP8266WebServer server(80);
WebInterface interface;

void setupSoftAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP8266 - xxx","abc123"); 
}

void setup(void)
{
  delay(1000);
  Serial.begin(74880);
  WiFi.begin(ssid, password);
  
  
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
  setupSoftAP();
  server.on("/", [](){
     interface.updateWiFiStatus();
    server.send(200, "text/html", interface.getHomePage());
  });
  server.on("/LED0_ON", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED0 ON");
    delay(1000);
  });
  server.on("/LED0_OFF", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED0 OFF");
    delay(1000); 
  });
   server.on("/LED1_ON", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED1 ON");
    delay(1000);
  });
  server.on("/LED1_OFF", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED1 OFF");
    delay(1000); 
  });
   server.on("/LED2_ON", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED2 ON");
    delay(1000);
  });
  server.on("/LED2_OFF", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED2 OFF");
    delay(1000); 
  });
   server.on("/LED3_ON", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED3 ON");
    delay(1000);
  });
  server.on("/LED3_OFF", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED3 OFF");
    delay(1000); 
  });
   server.on("/LED4_ON", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED4 ON");
    delay(1000);
  });
  server.on("/LED4_OFF", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED4 OFF");
    delay(1000); 
  });
   server.on("/LED5_ON", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED5 ON");
    delay(1000);
  });
  server.on("/LED5_OFF", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED5 OFF");
    delay(1000); 
  });
   server.on("/LED6_ON", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED6 ON");
    delay(1000);
  });
  server.on("/LED6_OFF", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED6 OFF");
    delay(1000); 
  });
   server.on("/LED7_ON", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED7 ON");
    delay(1000);
  });
  server.on("/LED7_OFF", [](){
    server.send(200, "text/html", interface.getHomePage());
    Serial.println("LED7 OFF");
    delay(1000); 
  });
   server.on("/RESET", [](){
    server.send(200, "text/html", interface.resetPage());
    ESP.reset();
    delay(1000); 
  });
  server.begin();
  Serial.println("HTTP server started");
}

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
} 
