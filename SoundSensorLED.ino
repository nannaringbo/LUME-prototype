#include <FastLED.h>

// Settings for LED Strip
#define NUM_LEDS 50
#define DATA_PIN 5
CRGB leds[NUM_LEDS];

// For calibration
const unsigned long calibrationDuration = 10000; // Calibration duration in milliseconds (30 seconds)
unsigned long startTime;                         // Variable to store the start time of calibration
int soundMin = 1023;                             // Initial minimum sound value, set to maximum possible value
int soundMax = 0;                                 // Initial maximum sound value, set to minimum possible value

// Settings for Sound Sensor
const int soundPin = A0;
int avgSound;

// Threshold values
int tMin;
int tMax;

// To keep track of the current state of the installation.
// 0 = Baseline, 1 = Accepted threshold, 2 = Noise pollution.
int currentState;
int lastState;


void setup() {
  delay(3000); // power-up safety delay
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(84);
  Serial.begin(9600);

  // Start calibration
  Serial.println("Calibrating sound sensor for 30 seconds...");
  startTime = millis();
}

void loop() {
  readSoundAndSetState(); // Read sound and set state
  
  
  // If calibration duration is not elapsed, read sound values
  if (millis() - startTime < calibrationDuration) {
    calibrateSoundSensor();
    Serial.print("tMin: ");
    Serial.print(tMin);
    Serial.print(" tMax: ");
    Serial.println(tMax);
    
  } 
  
  // Calibration is done, start LED animations
  else {
    if (currentState != lastState) {
      setCase();
    } else {
     
    }
    
  }
  
}

void calibrateSoundSensor() {
  int soundValue = analogRead(soundPin);
  
  if (soundValue < soundMin) {
    soundMin = soundValue; // Update soundMin if current value is lower
  }
  if (soundValue > soundMax) {
    soundMax = soundValue; // Update soundMax if current value is higher
  }
  tMin=(soundMax/4);
  tMax=(soundMax*2);
  
   
}

void readSoundAndSetState() {
  //  int sound1 = analogRead(soundPin);
  //  int sound2 = analogRead(soundPin);
  //  int sound3 = analogRead(soundPin);
  //  int sound4 = analogRead(soundPin);
  //  int sound5 = analogRead(soundPin);
   
   avgSound = analogRead(soundPin);
   
    
  
  if(avgSound < tMin){
    currentState=0;
  }
  else if(avgSound > tMax){
    currentState=2;
  }
  else{
    currentState=1;
  }
  
  
}

void setCase(){
    Serial.print(avgSound);
    Serial.print(" currentState: ");
    Serial.println(currentState);
  
 switch(currentState) {
   case 0: //Baseline
   lastState = 0;
   baselineAnimation();
   
   
     break;
   case 1: //Accepted Threshold
   lastState=1;
   
   acceptedThresholdAnimation();
   
   
     
     break;
   case 2: //Noise Pollution
   lastState=2;
     noisePollutionAnimation();
    break;
     
 }
}

  



void baselineAnimation() {
  // Set the LED color to warm white with a yellowish/orangeish tint
  CRGB warmWhite = CRGB(70, 150, 5);
  // Define fade parameters
  int fadeDelay = 10; // Delay between brightness changes

  static int brightness = 50; // Initial brightness

  // Fade in and fade out
  for (brightness=20; brightness <= 150; brightness++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = warmWhite;
      leds[i].nscale8(brightness);
    }
    FastLED.show();
    delay(fadeDelay);

    // Check the sound level again
    readSoundAndSetState();
    if (currentState != lastState) {
      return;
    }
  }

 for (brightness = 150; brightness >= 50; brightness--) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = warmWhite;
      leds[i].nscale8(brightness);
    }
    FastLED.show();
    delay(fadeDelay);

    // Check the sound level again
    readSoundAndSetState();
    if (currentState != lastState) {
      return;
    }
  }
}


void acceptedThresholdAnimation() {
   // Set the LED color to warm white with a yellowish/orangeish tint
  CRGB warmWhite = CRGB(70, 150, 5);
  
  static int brightness = 50; // Initial brightness
  
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = warmWhite;
      leds[i].nscale8(brightness);
    }
  
  static uint8_t hue = 0;
  for(int j = 0; j < 10; j++) {
    
  for(int i = 0; i < NUM_LEDS; i++) {
    fadeall();
    int randomNumber = random(0,5);
    leds[i] = CRGB(random(49,125), random(88, 170), random(64, 142));
    //leds[i] = CRGB(random(75,125), random(119, 170), random(92, 142));
    FastLED.show();
    delay(200);
    
    // Check the sound level after each LED is set
    readSoundAndSetState();
    if (currentState != lastState) {
      return; // Exit the loop if the state changes
    } 
    
  }
  
  }

}

void fadeall() { //Helper function to give a fading effect in the acceptedThresholdAnimation
  CRGB warmWhite = CRGB(30, 50, 5); 
  static int fadeAmount = 5;

  for (int i = 0; i < NUM_LEDS; i++) {
    if (leds[i].r < warmWhite.r) {
      
      leds[i].r += fadeAmount;
      
    } else if (leds[i].r > warmWhite.r) {
      leds[i].r -= fadeAmount;
    }

    if (leds[i].g < warmWhite.g) {
      leds[i].g += fadeAmount;
    } else if (leds[i].g > warmWhite.g) {
      leds[i].g -= fadeAmount;
    }

    if (leds[i].b < warmWhite.b) {
      leds[i].b += fadeAmount;
    } else if (leds[i].b > warmWhite.b) {
      leds[i].b -= fadeAmount;
    }
    
  }

  FastLED.show();
  delay(10); // Adjust the fading speed here
}


void noisePollutionAnimation(){ // LEDS fading out quickly, one by one.
 for(int i = (NUM_LEDS)-1; i >= -1; i--) {
   leds[i] = CRGB::Black;
   FastLED.show();
   // Wait a little bit before we loop around and do it again
   delay(10);
 }
 delay(2000);
 readSoundAndSetState();
    if (currentState != lastState) {
      return; // Exit the loop if the state changes
    }
}




