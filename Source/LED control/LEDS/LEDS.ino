/******************************************************************************
* PlanteRGB - Travis Hodgin
* Please make sure you are using the correct Controller in the Arduino IDE.
* Make sure your USB type is Serial, running on port 57600
* Input should look like: led_idx,red_value,green_value,blue_value
*******************************************************************************/
#include "FastLED.h"
#include "Arduino.h"

#define DEVICE_ID 1
#define NUM_LEDS 60 /* adjust to your length of LED */
#define DATA_PIN 2 /* adjust to the used pin (Arduino nano pin 2 = D2*/
#define rxPin 0 /* Reciever pin */
#define txPin 1 /* Transmitter pin */

CRGB leds[NUM_LEDS]; /*color object */
unsigned char io_port = 1; /* id of controller */
unsigned char led_idx = 0; /* starting index for LED change */
unsigned char r = 0, g = 0, b = 0; /* red, green, and blue values */

char readVal;
String readBuffer;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); /* initialize LEDs */
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(57600); /* serial port */

  /* Fancy setup animation */
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
}

void loop() {
  while (Serial.available() > 0) {
    readVal = Serial.read();
    readBuffer += readVal;
    
    if (readBuffer.length() >= 5) {
      io_port = readBuffer[0];
      if (io_port == DEVICE_ID) {
        led_idx = readBuffer[1];
        leds[led_idx].red = readBuffer[2];
        leds[led_idx].green = readBuffer[3];
        leds[led_idx].blue = readBuffer[4];  
        FastLED.show();
      }
      
      readBuffer="";
  
      Serial.write("ACK");
    }
  }
}
