/*************************************************************

  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - ESP32 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLyMJHCvpS"
#define BLYNK_DEVICE_NAME           "Nanoleaf"
#define BLYNK_AUTH_TOKEN            "bt_ip_GAwzXI-kFu9ccSS70qS6PTfW-g"


// Comment this out to disable prints and save space
//#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include "FastLED.h"
#define NUM_LEDS 54
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define PIN1 2
int data = 255;

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Noel";
char pass[] = "12345678";


int power = 1; // power on/off
int p = 0; // preset value
int br = 50; // brightness value
int m = 1; // mode
int srv = 0; // sound reactive value

int r = 255;
int g = 255;
int b = 255;

int l = 0;

int segmen = 9; // jumlah segmen
int segmenl = 6; // led tiap segmen

bool gReverseDirection = false;
#define FRAMES_PER_SECOND 30


BLYNK_WRITE(V1) { // sound reactive on
  Serial.println("sr on");
  // Set incoming value from pin V0 to a variable
  srv = param.asInt();
}


BLYNK_WRITE(V2) { // power on
  Serial.println("power on");
  // Set incoming value from pin V0 to a variable
  power = param.asInt();
  if (power == 1) {
    FastLED.setBrightness(br);
  }
  else {
    FastLED.setBrightness(0);
  }
}

BLYNK_WRITE(V12) { // colour picker
  Serial.print("color picker red");
  r = param.asInt();
}
BLYNK_WRITE(V14) { // colour picker
  Serial.print("green");
  g = param.asInt();
}
BLYNK_WRITE(V15) { // colour picker
  Serial.println("blue");
  b = param.asInt();
}

BLYNK_WRITE(V3) { // brightness
  Serial.println("brightness");
  br = param.asInt();
  FastLED.setBrightness(br);
}

BLYNK_WRITE(V4) { // Static mode
  Serial.println("static mode");
  m = 1;
  p = 0;
}
BLYNK_WRITE(V5) { // Running mode
  Serial.println("running mode");
  m = 2;
  p = 0;
}
BLYNK_WRITE(V7) { // Breathing mode
  Serial.println("Breathing mode");
  m = 3;
  p = 0;
}
BLYNK_WRITE(V13) { // Epileptic mode
  Serial.println("Epileptic mode");
  m = 4;
  p = 0;
}


BLYNK_WRITE(V8) { // Rainbow preset
  Serial.println("Rainbow preset");
  m = 0;
  p = 1;
}
BLYNK_WRITE(V9) { // fire preset
  Serial.println("fire preset");
  m = 0;
  p = 2;
}
BLYNK_WRITE(V10) { // Aurora preset
  Serial.println("Aurora preset");
  m = 0;
  p = 3;
}
BLYNK_WRITE(V11) { // Wave preset
  Serial.println("Aurora preset");
  m = 0;
  p = 4;
}


void static1() {
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB(r, g, b);
  }
}

void running1() {

  for (int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB(0, 0, 0);
  }
  for (int i = segmenl * l; i < segmenl * (l + 1); i++ )
  {
    leds[i] = CRGB(r/3, g/3, b/3);
  }
  if (l >= segmen-1){
    l = -1;
  }
  for (int i = segmenl * (l + 1); i < segmenl * (l + 2); i++ )
  {
    leds[i] = CRGB(r, g, b);
  }
  l = l + 1;
}


 
void breathing() {
  int hue = 0;
  int divisor = 30;
  float breath = (exp(sin(millis()/5000.0*PI)) - 0.36787944)*108.0;
  breath = map(breath, 0, 255, br/10, br);
  FastLED.setBrightness(breath);
  fill_rainbow(leds, NUM_LEDS, (hue++/divisor));
  if(hue == (255 * divisor)) {
     hue = 0;
  }
}


void epileptic() {
  if (l == 0) {
    l = 1;
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      leds[i] = CRGB(r, g, b);
    }
  }
  else {
    l = 0;
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      leds[i] = CRGB(0, 0, 0);
    }
  }
}












void setup() {
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  FastLED.addLeds<LED_TYPE, PIN1, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}

void loop() {
  Blynk.run();
  if (srv == 1){
    soundReactive();                                       // And then display it.
  }
  else{
    if (m == 1) {
      static1();
    }
    else if (m == 2) {
      running1();
    }
    else if (m == 3) {
      breathing();
    }
    else if (m == 4) {
      epileptic();
    }
    else if (p == 1) {
      rainbow();
    }
    else if (p == 2) {
      Fire2012();
    }
    else if (p == 3) {
      aurora();
    }
    else if (p == 4) {
      fairy();
    }
  delay(1000 / FRAMES_PER_SECOND);
  }
  FastLED.show();
}

#define COOLING  55
#define SPARKING 120
void Fire2012() { // fire preset
  // Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor( heat[j]);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}


uint8_t   noiseData[NUM_LEDS];

CRGBPalette16 party = PartyColors_p;

uint8_t   octaveVal   = 2;
uint16_t  xVal        = 0;
int       scaleVal    = 50;
uint16_t  timeVal     = 0;

void fairy() { // fairy preset
  timeVal = millis() / 4;

  memset(noiseData, 0, NUM_LEDS);
  fill_raw_noise8(noiseData, NUM_LEDS, octaveVal, xVal, scaleVal, timeVal);
  
  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette(party, noiseData[i], noiseData[NUM_LEDS - i - 1]);
  }
}


int hue = 0;
void rainbow() { // rainbow preset
  for (int i = 0; i < NUM_LEDS; i++) {
    //leds[i] = CHSV(hue, 255, 255);
    leds[i] = CHSV(hue + (i * 10), 255, 255);
  }

  EVERY_N_MILLISECONDS(15){
    hue++;
  }
}


uint8_t colorIndex[NUM_LEDS];

DEFINE_GRADIENT_PALETTE( greenblue_gp ) { 
  0,   0,  255, 245,
  46,  0,  21,  255,
  179, 12, 250, 0,
  255, 0,  255, 245
};

CRGBPalette16 greenblue = greenblue_gp;
void aurora() { // aurora preset
  // Color each pixel from the palette using the index from colorIndex[]
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(greenblue, colorIndex[i]);
  }
  
  EVERY_N_MILLISECONDS(5){
    for (int i = 0; i < NUM_LEDS; i++) {
      colorIndex[i]++;
    }
  }
}


void soundReactive(){
  
}
