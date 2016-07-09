/* ================================================================================================
         Project: Elezen.Band
          Author: Armin Nehzat
         Created: 10th July 2016
     Arduino IDE: 1.6.4
         Website: http://www.eronka.com
     Description: This sketch will give life to the neo pixel via microphone audio input.
================================================================================================== */

//This project needs the FastLED library - link in the description.
#include "FastLED.h"

//The total number of LEDs being used is 150
#define NUM_LEDS 16

// The data pin for the NeoPixel strip is connected to digital Pin 6 on the Arduino
#define DATA_PIN 6

//Attach the Grove Ear-clip heart rate sensor to digital pin 2 on the Arduino.
#define EAR_CLIP 8

//Initialise the LED array
CRGB leds[NUM_LEDS];

//Initialise the global variables used to control the LED animation
int ledNum = 0;           //Keep track of the LEDs
boolean beated = false;   //Used to identify when the heart has beated
int randomR = 0;          //randomR used to randomise the fade-out of the LEDs

//Initialize micraphone
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;


//Initialize touch pad
// Constants
const int TOUCH_BUTTON_PIN = 3;  // Input pin for touch state
const int ledPin = 13;          // Pin number for LED

// Global Variables
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;             // Variable for reading button
int previousState = 0;

int color = 0;

//================================================================================================
// setup() : Is used to initialise the LED strip
//================================================================================================
void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);

  //Set digital pin 2 (Ear-clip heart rate sensor) as an INPUT
  pinMode(EAR_CLIP, INPUT);

  // Configure button pin as input 
  pinMode(TOUCH_BUTTON_PIN, INPUT);
  
  // Configure LED pin as output
  pinMode(ledPin, OUTPUT);

}


//================================================================================================
// loop() : Take readings from the Ear-clip sensor, and display the animation on the LED strip
//================================================================================================
void loop() {

  // Read the state of the capacitive touch board
  buttonState = digitalRead(TOUCH_BUTTON_PIN);

  // compare states
  if (previousState != buttonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter++;
      //Serial.println("on");
      //Serial.print("number of button pushes:  ");
      //Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button
      // wend from on to off:
      //Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    //delay(200);
  }

  //save last reading to previous state
  previousState = buttonState;

  switch(buttonPushCounter) {
    case 0:
    color = 1;
    //Serial.println("red");
    break;
    case 1:
    color = 2;
    //Serial.println("green");
    break;
    case 2:
    color = 3;
    //Serial.println("blue");
    break;
    default:
    buttonPushCounter = 0;
    //Serial.println("NA");
  }

  //Serial.println("the final colour is");
  //Serial.println(color);


  //audio sample
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;

   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 3.3) / 1024;  // convert to volts

  //Serial.println(volts);
  //If the audio sensor moves from LOW to HIGH, call the beatTriggered method
  if(volts>2){

    //beatTriggered() is only called if the 'beated' variable is false.
    //This prevents multiple triggers from the same beat.
    if(!beated){
        beatTriggered(color);
    }
  } else {
    beated = false;  //Change the 'beated' variable to false when the Ear-clip heart rate sensor is reading LOW.
  }

  //Fade the LEDs by 1 unit/cycle, when the heart is at 'rest' (i.e. between beats)
  fadeLEDs(100);

}

//================================================================================================
// beatTriggered() : This is the LED animation sequence when the heart beats
//================================================================================================
void beatTriggered(int color){
  Serial.print("trigger ");  
  Serial.println(color);
  if (color == 1) {
    //Serial.println("red");  
      //Ignite 30 LEDs with a red value between 0 to 255
      for(int i = 0; i<16; i++){
        //The red channel is randomised to a value between 0 to 255
        leds[ledNum].r=random8();
        FastLED.show();
    
        //Call the fadeLEDs method after every 3rd LED is lit.
        if(ledNum%3==0){
          fadeLEDs(5);
        }
    
        //Move to the next LED
        ledNum++;
    
        //Make sure to move back to the beginning if the animation falls off the end of the strip
        if(ledNum>(NUM_LEDS-1)){
          ledNum=0;
        }
      }    
  }
  else if (color == 2) {
    //Serial.println("green");  
      //Ignite 30 LEDs with a red value between 0 to 255
      for(int i = 0; i<16; i++){
        //The red channel is randomised to a value between 0 to 255
        leds[ledNum].g=random8();
        FastLED.show();
    
        //Call the fadeLEDs method after every 3rd LED is lit.
        if(ledNum%3==0){
          fadeLEDs(5);
        }
    
        //Move to the next LED
        ledNum++;
    
        //Make sure to move back to the beginning if the animation falls off the end of the strip
        if(ledNum>(NUM_LEDS-1)){
          ledNum=0;
        }
      }    
  }
  else {
    //Serial.println("blue");
      //Ignite 30 LEDs with a red value between 0 to 255
      for(int i = 0; i<16; i++){
        //The red channel is randomised to a value between 0 to 255
        leds[ledNum].b=random8();
        FastLED.show();
    
        //Call the fadeLEDs method after every 3rd LED is lit.
        if(ledNum%3==0){
          fadeLEDs(5);
        }
    
        //Move to the next LED
        ledNum++;
    
        //Make sure to move back to the beginning if the animation falls off the end of the strip
        if(ledNum>(NUM_LEDS-1)){
          ledNum=0;
        }
      }    
  }


  //Change the 'beated' variable to true, until the Ear-Clip sensor reads LOW.
  beated=true;
}


//================================================================================================
// fadeLEDs() : The fading effect of the LEDs when the Heart is resting (Ear-clip reads LOW)
//================================================================================================
void fadeLEDs(int fadeVal){
  for (int i = 0; i<NUM_LEDS; i++){
    //Fade every LED by the fadeVal amount
    leds[i].fadeToBlackBy( fadeVal );

    //Randomly re-fuel some of the LEDs that are currently lit (1% chance per cycle)
    //This enhances the twinkling effect.
    if(leds[i].r>10){
      randomR = random8(100);
      if(randomR<1){
        //Set the red channel to a value of 80
        leds[i].r=80;
        //Increase the green channel to 20 - to add to the effect
        leds[i].g=20;
      }
    }
  }
  FastLED.show();
}
