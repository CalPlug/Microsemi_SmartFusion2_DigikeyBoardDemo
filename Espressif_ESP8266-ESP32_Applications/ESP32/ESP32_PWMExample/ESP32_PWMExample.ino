//ESP32 PWM Example (Generate a tone on a Piezo element)

int freq = 2000;
int channel = 0;
int resolution = 8;
 
void setup() {
 
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);
 
}
 
void loop() {
 
  ledcWriteTone(channel, 2000);
 
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle=dutyCycle+10){
 
    Serial.println(dutyCycle);
 
    ledcWrite(channel, dutyCycle);
    delay(1000);
  }
 
  ledcWrite(channel, 125);
 
  for (int freq = 255; freq < 10000; freq = freq + 250){
 
     Serial.println(freq);
 
     ledcWriteTone(channel, freq);
     delay(1000);
  }
 
}
 
