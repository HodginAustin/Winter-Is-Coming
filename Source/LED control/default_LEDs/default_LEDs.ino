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

#define NUM_LEDS 60       // adjust to your length of LED strip*/
#define LED_DATA_PIN 2    // adjust to the used pin (Arduino nano pin 2 = D2)
#define DEVICE_ID 3       // DEVICE_ID ranges from min (Pi reserves 0-2) 3 
                          // to max 127 (7 bit addresses, defined by Atmel C libs)

CRGB leds[NUM_LEDS];      // color object for setup testing
unsigned char led_idx;

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);  // initialize LEDs
  //Serial.begin(9600);                                     // open the serial port at 9600 bps 
                                                            // to send to the serial monitor for debugging if necessary
  Wire.onReceive(receiveEvent);                             // Action upon recieving data (as interrupt)
  Wire.setClock(400000);

  // Fancy setup animation & test each LED
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

  Wire.begin(DEVICE_ID);                                // Set Arduino up as an I2C slave at address defined above
  
  //Serial.println("Ready!");
}

void loop() {   // Do nothing until I2C serial arrives
}

void receiveEvent(int numBytes) {     // At I2C rx interrupt, do this

  if (numBytes == 4) {

    while(Wire.available() > 0) {

      led_idx = Wire.read();
      leds[led_idx].red = Wire.read();
      leds[led_idx].green = Wire.read();
      leds[led_idx].blue = Wire.read();
      FastLED.show();

      //Serial.println("#");
      //Serial.println(led_idx);
      //Serial.println(leds[led_idx].red);
      //Serial.println(leds[led_idx].green);
      //Serial.println(leds[led_idx].blue);
    }
  }
}
