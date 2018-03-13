/******************************************************************************
* PlanteRGB - Austin Hodgin, Travis Hodgin, Max Schmidt, Zach Lerew
* Please make sure you are using the correct Controller in the Arduino IDE.
* Make sure your USB type is Serial, running at speed 9600
* Input should look like: led_idx,red_value,green_value,blue_value as a 
* series of bytes
*******************************************************************************/
#include <Wire.h>
#include "Arduino.h"
#include "FastLED.h"

#define NUM_LEDS 60   /* adjust to your length of LED strip*/
#define DATA_PIN 2    /* adjust to the used pin (Arduino nano pin 2 = D2) */

CRGB leds[NUM_LEDS];  /*color object */

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // initialize LEDs
  Serial.begin(9600);             // open the serial port at 9600 bps for serial monitor
  Wire.begin(7);                  // Set Arduino up as an I2C slave at address 0x07
  Wire.onReceive(receiveEvent);   // Action upon recieving data

  /* Fancy setup animation & test each LED */
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i].red = 10;
    leds[i].green = 0;
    leds[i].blue = 0;
    FastLED.show();
    delay(5);
  }
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i].red = 0;
    leds[i].green = 10;
    leds[i].blue = 0;
    FastLED.show();
    delay(5);
  }
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i].red = 0;
    leds[i].green = 0;
    leds[i].blue = 10;
    FastLED.show();
    delay(5);
  }
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(5);
  }
  delay(1000);  
  
  Serial.println("Ready!");
}

void loop() {
  delay(1);
}

void receiveEvent(int numBytes){  

  while(Wire.available() > 0) {
    led_idx = Wire.read();
    leds[led_idx].red = Wire.read();
    leds[led_idx].green = Wire.read();
    leds[led_idx].blue = Wire.read();
    FastLED.show();
  }
}
