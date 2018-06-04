//----RTC----
#include "RTClib.h"
RTC_DS3231 rtc;
//----RTC----

//----DISPLAY----
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
const byte PIN_CLK = 5;   // define CLK pin
const byte PIN_DIO = 3;   // define DIO pin
SevenSegmentExtended      display(PIN_CLK, PIN_DIO);
//----DISPLAY----

//----NEOPIXELS----
#include <Adafruit_NeoPixel.h>
#define NUM_PIX 40         // define number of pixels
#define HR_PIX 24         // define number of hour hand pixels
#define MIN_PIX 16         // define number of minute hand pixels
#define PIN_PIX 7          // define Neopixel pin
const byte NGT_PIX = 80;   // define Neopixel night brightness (0-255)
const byte DAY_PIX = 160;   // define Neopixel day brightness (0-255)
byte fadedir = 1;
uint16_t fadelevel = 100;
byte fadedir2 = 0;
uint16_t fadelevel2 = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIX, PIN_PIX, NEO_GRB + NEO_KHZ800);

//1 = RED, 2= YELLOW, 3= GREEN, 4=BLUE, 5=PURPLE
//               1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
int hourmap[]  = {1, 2, 2, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 1, 1, 1};
//               12  12 1  1  2  2  3  3  4  4  5  5  6  6  7  7  8  8  9  9  10 10 11 11
int nghtmap[]  = {1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 1, 1, 1, 1};
uint16_t pix_red[] = {255,  0,  0};
uint16_t pix_yel[] = {155,100,  0};
uint16_t pix_grn[] = { 35,210, 10};
uint16_t pix_blu[] = {  0,  0,255};
uint16_t pix_pur[] = {150,  0,100};
uint16_t pix_cyn[] = {37,  37,180};
uint16_t pix_pnk[] = {180, 37, 37};
//----NEOPIXELS----

void setup() {
  Serial.begin(9600);         // initializes the Serial connection @ 9600 baud
  
  //----RTC----
  if (! rtc.begin()) {                // check if RTC is up and running
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.print("Adjusting RTC time to "); // set RTC time to PC time
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //----RTC----

  //----NEOPIXELS----
  byte i;
  pixels.begin();
  //for(i=0; i< NUM_PIX; i++) { // sets all pixels to red for testing
  //  pixels.setPixelColor(i, 255,000,000);
  //}
  pixels.show(); //Sets pixels to off on startup
  //delay(1000);
  //----NEOPIXELS----

  //----DISPLAY----
  display.setBacklight(20);   // set the brightness
  display.begin();            // initializes the display
  //----DISPLAY----
}

void loop() {
  //----RTC----
  DateTime now = rtc.now();   // read time from RTC
  //Serial.print("now: ");
  //Serial.print(now.hour(), DEC);
  //Serial.print(':');
  //Serial.print(now.minute(), DEC);
  //Serial.print(':');
  //Serial.print(now.second(), DEC);
  //Serial.println();
  //----RTC----

  //----DISPLAY----
  byte hours    = now.hour();                          // setting hours 
  byte minutes  = now.minute();                        // setting minutes
  //hours    = 12;                                      // test value
  //minutes  = 00;                                     // test value
  byte twelve_hours=hours;                             // setting twelve_hours to real time
  if (twelve_hours>12) {twelve_hours=twelve_hours-12;} // convert to 12hr time
  if (twelve_hours==0) {twelve_hours=12;}              // convert to 0hr to 12
  display.printTime(twelve_hours, minutes, false);     // display time
  //----DISPLAY----

  //----NEOPIXELS----
  byte i, j;
  uint16_t hour_toshow = twelve_hours * HR_PIX / 12 ; // Determining which pixel to light
  if (minutes>=30) {hour_toshow = hour_toshow + 1;}
  if (hour_toshow>HR_PIX) {hour_toshow = hour_toshow - 24;}
  if (hour_toshow==24) {hour_toshow = 0;}
  uint16_t minute_toshow = minutes * MIN_PIX / 60;
  //Serial.print("Hour LED:")
  //Serial.println(hour_toshow);
  //Serial.print("Minute LED:")
  //Serial.println(minute_toshow);
  //delay(500);
  
  // AWAKE COLORING
  if (hourmap[hours]==4) { 
    wakeyLights(hour_toshow, minute_toshow, 5);
    //Serial.print("Awake Lighting (Hr, Mn): ");
  }
  
  // BEDTIME  COLORING
  if (hourmap[hours] != 4) { 
    sleepyLights(hour_toshow, minute_toshow, 10);
    //Serial.print("Bedtime Lighting (Hr, Mn): ");
  }
  //----NEOPIXELS----
}

void wakeyLights(byte hour_toshow, byte minute_toshow, byte wait) {
  byte i, j, k;
  pixels.setBrightness(DAY_PIX);
  for(i=0; i<NUM_PIX; i++) {
    if(i<HR_PIX){ // HOUR RING
      pixels.setPixelColor(i,pix_pnk[0]/16,pix_pnk[1]/16,pix_pnk[2]/16);
      //Serial.println(hour_toshow);
      //Serial.println(i);
      if(i==hour_toshow){ // FADE CORRECT HOUR LIGHT
        if (fadedir==0){
          //Serial.println("FAD_HR_UP");
          fadelevel++;
          if (fadelevel==100){fadedir=1;}
        }
        if (fadedir==1){
          //Serial.println("FAD_HR_DN");
          fadelevel--;
          if (fadelevel==0){fadedir=0;}
        }
        pixels.setPixelColor(i,pix_pnk[0]*fadelevel/150,pix_pnk[1]*fadelevel/150,pix_pnk[2]*fadelevel/150);
      }  
    }
    if(i>=HR_PIX){ // MINUTE RING
      pixels.setPixelColor(i,pix_cyn[0]/8,pix_cyn[1]/8,pix_cyn[2]/8);
      if(i==HR_PIX+minute_toshow){ // FADE CORRECT MINUTE LIGHT
        if (fadedir2==0){
          fadelevel2++;
          if (fadelevel2==100){fadedir2=1;}
        }
        if (fadedir2==1){
          fadelevel2--;
          if (fadelevel2==0){fadedir2=0;}
        }
        pixels.setPixelColor(i,pix_cyn[0]*fadelevel/150,pix_cyn[1]*fadelevel/150,pix_cyn[2]*fadelevel/150);
      }
    }
  }
  pixels.show();
  delay(wait);
}

void sleepyLights(byte hour_toshow, byte minute_toshow, byte wait) {
  byte i, j, k;
  pixels.setBrightness(NGT_PIX);
  //Serial.println(minute_toshow);
  for(i=0; i<NUM_PIX; i++) {
    if(i<HR_PIX){ // HOUR RING
      if(nghtmap[i]==1){pixels.setPixelColor(i,pix_red[0]/8,pix_red[1]/8,pix_red[2]/8);}
      if(nghtmap[i]==2){pixels.setPixelColor(i,pix_yel[0]/8,pix_yel[1]/8,pix_yel[2]/8);}
      if(nghtmap[i]==3){pixels.setPixelColor(i,pix_grn[0]/8,pix_grn[1]/8,pix_grn[2]/8);}
      if(nghtmap[i]==4){pixels.setPixelColor(i,pix_blu[0]/8,pix_blu[1]/8,pix_blu[2]/8);}
      if(nghtmap[i]==5){pixels.setPixelColor(i,pix_pur[0]/8,pix_pur[1]/8,pix_pur[2]/8);}
      if(i==hour_toshow){ // FADE CORRECT HOUR LIGHT
        if (fadedir==0){
          //Serial.println("FADE_HR_UP");
          //Serial.println(fadedir);
          fadelevel++;
          if (fadelevel==100){fadedir=1;}
        }
        if (fadedir==1){
          //Serial.println("FADE_HR_DN");
          //Serial.println(fadedir);
          fadelevel--;
          if (fadelevel==0){fadedir=0;}
        }
        if(nghtmap[i]==1){pixels.setPixelColor(i,pix_red[0]*fadelevel/150,pix_red[1]*fadelevel/150,pix_red[2]*fadelevel/150);}
        if(nghtmap[i]==2){pixels.setPixelColor(i,pix_yel[0]*fadelevel/150,pix_yel[1]*fadelevel/150,pix_yel[2]*fadelevel/150);}
        if(nghtmap[i]==3){pixels.setPixelColor(i,pix_grn[0]*fadelevel/150,pix_grn[1]*fadelevel/150,pix_grn[2]*fadelevel/150);}
        if(nghtmap[i]==4){pixels.setPixelColor(i,pix_blu[0]*fadelevel/150,pix_blu[1]*fadelevel/150,pix_blu[2]*fadelevel/150);}
        if(nghtmap[i]==5){pixels.setPixelColor(i,pix_pur[0]*fadelevel/150,pix_pur[1]*fadelevel/150,pix_pur[2]*fadelevel/150);}
      }
    }
    if(i>=HR_PIX){ // MINUTE RING
      if(nghtmap[hour_toshow]==1){pixels.setPixelColor(i,pix_red[0]/16,pix_red[1]/16,pix_red[2]/16);}
      if(nghtmap[hour_toshow]==2){pixels.setPixelColor(i,pix_yel[0]/16,pix_yel[1]/16,pix_yel[2]/16);}
      if(nghtmap[hour_toshow]==3){pixels.setPixelColor(i,pix_grn[0]/16,pix_grn[1]/16,pix_grn[2]/16);}
      if(nghtmap[hour_toshow]==4){pixels.setPixelColor(i,pix_blu[0]/16,pix_blu[1]/16,pix_blu[2]/16);}
      if(nghtmap[hour_toshow]==5){pixels.setPixelColor(i,pix_pur[0]/16,pix_pur[1]/16,pix_pur[2]/16);}
      if(i==HR_PIX+minute_toshow){ // FADE CORRECT MINUTE LIGHT
        if (fadedir2==0){
          fadelevel2++;
          if (fadelevel2==100){fadedir2=1;}
        }
        if (fadedir2==1){
          fadelevel2--;
          if (fadelevel2==0){fadedir2=0;}
        }
        if(nghtmap[hour_toshow]==1){pixels.setPixelColor(i,pix_red[0]*fadelevel/150,pix_red[1]*fadelevel/150,pix_red[2]*fadelevel/150);}
        if(nghtmap[hour_toshow]==2){pixels.setPixelColor(i,pix_yel[0]*fadelevel/150,pix_yel[1]*fadelevel/150,pix_yel[2]*fadelevel/150);}
        if(nghtmap[hour_toshow]==3){pixels.setPixelColor(i,pix_grn[0]*fadelevel/150,pix_grn[1]*fadelevel/150,pix_grn[2]*fadelevel/150);}
        if(nghtmap[hour_toshow]==4){pixels.setPixelColor(i,pix_blu[0]*fadelevel/150,pix_blu[1]*fadelevel/150,pix_blu[2]*fadelevel/150);}
        if(nghtmap[hour_toshow]==5){pixels.setPixelColor(i,pix_pur[0]*fadelevel/150,pix_pur[1]*fadelevel/150,pix_pur[2]*fadelevel/150);}
      }
    }
  }
  pixels.show();
  delay(wait);
}
