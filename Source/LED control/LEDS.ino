#include "FastLED.h"
#include "Arduino.h"
#define NUM_LEDS 14
#define DATA_PIN 0

CRGB leds[NUM_LEDS]; /*color object */
int start_led = 0;
int end_led  = 0;
int r = 0, g = 0, b = 0;


void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  randomSeed(255);
  Serial.begin(57600);
}


void loop(){
  int i;
  int j;
  FastLED.show();
  Serial.flush();
  if(Serial.available() > 0){
    Serial.println("Procressing....");
    delay(100);
    start_led = Serial.parseInt();
    end_led = Serial.parseInt();
    r = Serial.parseInt();
    g = Serial.parseInt();
    b = Serial.parseInt();
    Serial.print(r);
    for(int i = start_led; i < end_led; i++){
      leds[i].red = r;
      leds[i].green = g;
      leds[i].blue = b;
    }
   
  }
}
