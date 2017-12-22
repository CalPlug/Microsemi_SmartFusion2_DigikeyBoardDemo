int ledPin = 1;  //The LED is GPIO on the ESP01, 13 on the NodeMCU  Avoid GPIO 2 or GPIO 0 on the ESP8266 as these are used in a number of designs as control inputs and the status of these pins may have an effect on bootup.

void setup()
{
    pinMode(ledPin, OUTPUT);
    Serial.begin(115200);
}

void loop()
{
    Serial.println("Hello, world!");
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
}
