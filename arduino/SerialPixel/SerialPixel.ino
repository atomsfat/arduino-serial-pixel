 /*
 * SerialPixel is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * PixelController is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 	
 */

#include <TimerOne.h>  
#include "LPD6803.h"

#define BAUD_RATE 115200

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
int dataPin = 2;       // 'green' wire
int clockPin = 3;      // 'blue' wire
// Don't forget to connect 'blue' to ground and 'red' to +5V

//initialize strip with 20 leds
LPD6803 strip = LPD6803(20, dataPin, clockPin);
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int ledLights[20];


unsigned int Color(byte r, byte g, byte b) {
  //Take the lowest 5 bits of each value and append them end to end
  return( ((unsigned int)g & 0x1F )<<10 | ((unsigned int)b & 0x1F)<<5 | (unsigned int)r & 0x1F);
}


//create initial image
void showInitImage() {
    for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
    }    
}


// --------------------------------------------
//      setup
// --------------------------------------------
void setup() {
  //im your slave and wait for your commands, master!
  Serial.begin(BAUD_RATE); //Setup high speed Serial
  Serial.flush();
  inputString.reserve(200);

  strip.setCPUmax(70);  // start with 50% CPU usage. up this if the strand flickers or is slow

  // Start up the LED counter
  strip.begin();
  
  showInitImage();
  
  // Update the strip, to start they are all 'off'
  strip.show();
  establishContact();
}

// --------------------------------------------
//      loop
// --------------------------------------------
int my_integer_data = 0;
void loop() {
 
   if (stringComplete) {
    //Serial.println(inputString); 
    
    //Serial.println(inputString.substring(0,2).toInt()) ;
    //Serial.println(inputString.substring(2,5).toInt()) ;
    //Serial.println(inputString.substring(5,8).toInt()) ;
    //Serial.println(inputString.substring(8,11).toInt()) ;
     
     int c;
     int index = inputString.substring(0,2).toInt();
     int red = inputString.substring(2,5).toInt();
     int green = inputString.substring(5,8).toInt();
     int blue = inputString.substring(8,11).toInt();
          
     c = Color(red, green, blue);
      
     if(index < strip.numPixels()){
       ledLights[index]= c;
     }
      
    for (byte i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i,ledLights[i]);   
    }
    
    strip.doSwapBuffersAsap(20);   
    Serial.println("more");
    inputString = "";
    stringComplete = false;  
  }
 
   //  strip.show();   // write all the pixels out
   
 // delay(20);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}


void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("A");   // send a capital A
    delay(300);
  }
}
