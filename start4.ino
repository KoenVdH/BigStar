#include <Adafruit_NeoPixel.h>

#define PIN 2

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(200, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  Serial.begin(115200);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint32_t Wheel30Percent(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(85 - WheelPos, 0, WheelPos);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos, 85 - WheelPos);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos, 85 - WheelPos, 0);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void turnRight(){
  uint32_t c = strip.getPixelColor(strip.numPixels()-1);
  for(int i=strip.numPixels()-1; i>0; i--) {
      strip.setPixelColor(i, strip.getPixelColor(i-1));
    }
   strip.setPixelColor(0, c);
}

void cycleRight(uint16_t cycles, uint8_t wait){
  for(int k=0; k<cycles; k++){
    delay(wait);
    turnRight();
    strip.show();
  }
  
}

void turnLeft(){
  uint32_t c = strip.getPixelColor(0);
  for(int i=0; i< strip.numPixels()-1; i++) {
      strip.setPixelColor(i, strip.getPixelColor(i+1));
    }
   strip.setPixelColor(strip.numPixels()-1, c);
}

void cycleLeft(uint16_t cycles, uint8_t wait){
  for(int k=0; k<cycles; k++){
    delay(wait);
    turnLeft();
    strip.show();
  }
}

void startSetOff(){
  for(int l=0; l<200;l++){
    strip.setPixelColor(l,strip.Color(0, 0, 0));
  }
}

void startSetDim(uint8_t back){
  for(int l=0; l<200;l++){
    strip.setPixelColor(l,strip.Color(back, back, back));
  }
}

void startSetBlue(){
  startSetOff();
  
  for(int l=0; l<200;l=l+25){
    strip.setPixelColor(l,strip.Color(0, 0, 127));
    strip.setPixelColor(l+1,strip.Color(0, 0, 60));
    strip.setPixelColor(l+2,strip.Color(0, 0, 20));
    strip.setPixelColor(l+3,strip.Color(0, 0, 10));
  }
}

void startTips(){
  for(int m=0; m<21;m++){
    
    startSetDim(10);
    
    for(int l=0; l<5;l++){
      strip.setPixelColor(l*40+m,strip.Color(127, 127, 127));
     }
     
     for(int l=1; l<5;l++){
      strip.setPixelColor(l*40-m,strip.Color(127, 127, 127));
     }
     strip.show();
     delay(25);
  }
}

void startSetRednBlue(){
  //startSetOff();
  startSetDim(5);
  
  for(int l=0; l<200;l=l+40){
    // blue
    strip.setPixelColor(l,strip.Color(0, 0, 255));
    strip.setPixelColor(l+1,strip.Color(0, 0, 120));
    strip.setPixelColor(l+2,strip.Color(0, 0, 60));
    strip.setPixelColor(l+3,strip.Color(0, 0, 20));
    strip.setPixelColor(l+4,strip.Color(0, 0, 10));
    // red
    strip.setPixelColor(l+25,strip.Color(0, 255, 0));
    strip.setPixelColor(l+25+1,strip.Color(0, 120, 0));
    strip.setPixelColor(l+25+2,strip.Color(0, 60, 0));
    strip.setPixelColor(l+25+3,strip.Color(0, 20, 0));
    strip.setPixelColor(l+25+4,strip.Color(0, 10, 0));
  }
}

void startSetStarPoints(){
  startSetOff();
  
  for(int l=0; l<20;l++){
    // blue
    strip.setPixelColor(l,strip.Color(0, 0, 255));
    strip.setPixelColor(l+40,strip.Color(0, 0, 255));
    strip.setPixelColor(l+80,strip.Color(0, 0, 255));
    strip.setPixelColor(l+120,strip.Color(0, 0, 255));
    strip.setPixelColor(l+160,strip.Color(0, 0, 255));
    // red
    strip.setPixelColor(l+20,strip.Color(0, 255, 0));
    strip.setPixelColor(l+60,strip.Color(0, 255, 0));
    strip.setPixelColor(l+100,strip.Color(0, 255, 0));
    strip.setPixelColor(l+140,strip.Color(0, 255, 0));
    strip.setPixelColor(l+180,strip.Color(0, 255, 0));
  }
}

void setRainBow(){
  startSetOff();
  uint32_t t;
  
  for(int l=0; l<200; l++){
    t = 255 * l / 200;
    strip.setPixelColor(l, Wheel30Percent(t));
  }
}

void setRainBowPart(uint8_t width,uint8_t start){
  startSetOff();
  uint32_t t;
  
  for(int l=0; l<100; l++){
    t = width * l / 100;
    strip.setPixelColor(l, Wheel30Percent(t+start));
  }

  for(int l=0; l<100; l++){
    strip.setPixelColor(l+100, strip.getPixelColor(100-l-1));
  }
}


void inverse(){
  uint32_t tempColors[200];
  
  for(int l=0; l<200; l++){
    tempColors[l] = strip.getPixelColor(199-l);
  } 

  for(int l=0; l<200; l++){
    strip.setPixelColor(l,tempColors[l]);
  } 
}

void dimmer(uint8_t factor){
  for(int l=0; l<200; l++){
    // niet correct, moet per kleur dimmen
    strip.setPixelColor(l,strip.getPixelColor(l)/factor);
  } 
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, 255), 50); // Blue

  // Send a theater pixel chase in
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue

  // rainbows
  //rainbow(5);
  //rainbowCycle(5);
  //theaterChaseRainbow(50);

  // rainbowparts
  // red yellow pink
  //setRainBowPart(40,60);
  // purple blue
  //setRainBowPart(40,120);

  //startSetRednBlue();


  // GEEL ORANGE DIM
  //for(int l=35; l<45 ; l = l + 2){
  for(int l=0; l<215 ; l = l + 5){
    //Serial.print(l);
    //Serial.println(" start");
    setRainBowPart(40,l);
    cycleRight(200,25);
    cycleLeft(200,25);  
  }
  
  // RED N BLUE RUNNING
  startSetRednBlue();
  inverse();
  cycleRight(2000,25);


  //startSetStarPoints();
  //startSetDim(10);
  //strip.setPixelColor(0,strip.Color(255, 255, 255));
  //for(int l=0; l<10 ; l++){
  //  cycleRight(400,0);
  //  cycleLeft(400,0);
  //}

  for(int l=215; l>0 ; l = l - 5){
    //Serial.print(l);
    //Serial.println(" start");
    setRainBowPart(40,l);
    cycleRight(200,25);
    cycleLeft(200,25);  
  }
  
  // WIT RUNNING
  for(int l=0; l<250;l++)
  {
    startTips();
    cycleRight(20,25);
    cycleLeft(20,25);
  }

}
