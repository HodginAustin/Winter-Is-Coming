/******************************************************************************
 * PlanteRGB - Travis Hodgin
 * Please make sure you are using the correct Controller in the Arduino IDE.
 * Make sure your USB type is Serial, running on port 57600
 * Input should look like: start_led,end_led,red_value,green_value,blue_value
 *****************************************************************************/
 
#include "FastLED.h"
#include "Arduino.h"
#define NUM_LEDS 12 /* adjust to your length of LED */
#define DATA_PIN 2 /* adjust to the used pin (Arduino nano pin 2 = D2*/

CRGB leds[NUM_LEDS]; /*color object */
int start_led = 0; /* starting index for LED change */
int end_led  = 0; /* ending index for LED change */
int r = 0, g = 0, b = 0; /* red, green, and blue values */


void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); /* initialize LEDs */
  Serial.begin(57600); /* serial port */
}


void loop(){
  Serial.flush(); /* clear buffer */
  /*if serial buffer contains something */
  if(Serial.available() > 0){
    Serial.println("Procressing....\n"); /* making sure it gets into */
    delay(100); /* wait to make sure everything is fully in buffer. */
    
    /*parseInt uses comma delimiter */ 
    start_led = Serial.parseInt();
    end_led = Serial.parseInt();
    r = Serial.parseInt();
    g = Serial.parseInt();
    b = Serial.parseInt();
    for(int i = start_led; i <= end_led; i++){
      leds[i].red = r;
      leds[i].green = g;
      leds[i].blue = b;
      FastLED.show();
    }
  }
}
