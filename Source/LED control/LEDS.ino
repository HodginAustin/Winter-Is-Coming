/******************************************************************************
* PlanteRGB - Travis Hodgin
* Please make sure you are using the correct Controller in the Arduino IDE.
* Make sure your USB type is Serial, running on port 57600
* Input should look like: led_idx,red_value,green_value,blue_value
*******************************************************************************/
#include "FastLED.h"
#include "Arduino.h"

#define NUM_LEDS 12 /* adjust to your length of LED */
#define DATA_PIN 2 /* adjust to the used pin (Arduino nano pin 2 = D2*/
#define rxPin 0 /* Reciever pin */
#define txPin 1 /* Transmitter pin */

CRGB leds[NUM_LEDS]; /*color object */

int led_idx = 0; /* starting index for LED change */
int r = 0, g = 0, b = 0; /* red, green, and blue values */

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); /* initialize LEDs */
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(57600); /* serial port */
  FastLED.show();
}
void loop() {
  while(Serial.available() > 0) {
//    led_idx = Serial.read();
      delay(100);
//    r = Serial.read();
//    g = Serial.read();
//    b = Serial.read();
      
//    leds[led_idx].red = r;
//    leds[led_idx].green = g;
//    leds[led_idx].blue = b;
//    FastLED.show();
     char temp = Serial.read();
     Serial.println(temp);
    Serial.flush();
  }
}

