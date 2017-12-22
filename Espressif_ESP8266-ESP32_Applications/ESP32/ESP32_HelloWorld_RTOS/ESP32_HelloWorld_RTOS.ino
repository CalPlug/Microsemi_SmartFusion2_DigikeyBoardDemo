//Sample RTOS Demo for the EspressIf ESP32 in Arduino IDE

//Michael Klopfer, Ph.D. 
//University of California, Irvine

//Based on the example:  https://exploreembedded.com/wiki/Hello_World_with_ESP32_Explained
//Also see:  https://www.hackster.io/mjrobot/iot-made-simple-playing-with-the-esp32-on-arduino-ide-0fe58c

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
 
#define BLINK_GPIO 13

void setup()
{
  Serial.begin(115200);  //begin Serial for interface
  pinMode(BLINK_GPIO, OUTPUT);
  digitalWrite(BLINK_GPIO, LOW);
}

 void a_task(void *pvParameter)
{
    printf("Hello world!\n");  //using STDIO to print via serial (equiv. to Serial.print())
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_RATE_MS);  //Delay via ticks in ESP32
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
 
void hello_task(void *pvParameter)
{
 
  while(1)
  {
      printf("Hello world!\n");
      vTaskDelay(100 / portTICK_RATE_MS);
  }
}
 
void blinky(void *pvParameter)
{
     while(1) {
        /* Blink off (output low) */
        digitalWrite(BLINK_GPIO, LOW);
        vTaskDelay(1000 / portTICK_RATE_MS);
        /* Blink on (output high) */
        digitalWrite(BLINK_GPIO, HIGH);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
 
 
void loop()
{
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&a_task, "a_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blinky, "blinky", 512,NULL,5,NULL );
     while (1)
    {} //Just loop after first run, treat this as a main() function rather than a loop
}
