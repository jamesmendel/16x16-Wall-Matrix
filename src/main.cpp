#include <Arduino.h>
#include <ESP8266Wifi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WifiManager.h>    //branch asyncwebserver
// #include <NeoPixelBrightnessBus.h>
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#include <LittleFS.h>
#include <full_site.h>

#define DEBUG TRUE

WiFiManager wm;
AsyncWebServer server(80);

const char* PARAM_INPUT_1 = "pattern";
String current_pattern = "off";

const uint8_t MatrixWidth  = 16;
const uint8_t MatrixHeight = 16;
const uint16_t PixelCount = MatrixWidth * MatrixHeight;
CRGB leds[PixelCount];
#define DATA_PIN 5
#define CHIPSET WS2812B
#define COLOR_ORDER GBR
#define MILLI_AMPS 10000    //10A power supply
#define BRIGHTNESS 40
#define FRAMES_PER_SECOND  120

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (MatrixWidth - 1) - x;
      i = (y * MatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * MatrixWidth) + x;
    }  
  return i;
}

// NeoTopology<RowMajorAlternatingLayout> topo(MatrixWidth, MatrixHeight);
// NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);
void renderMode();

void setup() {
  
  #ifdef DEBUG  //All debug-only scripts can go here
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    // WiFi.disconnect(true);
  #endif

/* -------------------------------------------------------------------------- */
/*                              Init WifiManager                              */
/* -------------------------------------------------------------------------- */

  wm.disconnect();
  wm.setAPStaticIPConfig(IPAddress(192,168,0,1), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
  wm.setMinimumSignalQuality(15);
  wm.setClass("invert");      //sets wm to dark mode
  wm.autoConnect("MATRIX");   //setting broadcast ssid and password when saved fallback creds fail.
  
  // WifiManager halts here until a network is connected succesfully
  // consider wifiManager.setConfigPortalTimeout(180);


/* -------------------------------------------------------------------------- */
/*                                   Routes                                   */
/* -------------------------------------------------------------------------- */

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", data_index_html);
    #ifdef DEBUG
      Serial.println("GET: /");
    #endif
  });
  server.on(data_style_css_path, HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/css", data_style_css);
    #ifdef DEBUG
      Serial.print("GET: ");
      Serial.println(data_style_css_path);
    #endif
  });
  server.on(data_script_js_path, HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/javascript", data_script_js);
    #ifdef DEBUG
      Serial.print("GET: ");
      Serial.println(data_script_js_path);
    #endif
  });

/* ----------------------------------- API ---------------------------------- */

  server.on("/mode", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      current_pattern = inputMessage;
      renderMode();
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(current_pattern).c_str());
  });

/* ------------------------------ Start server ------------------------------ */

  server.begin();

/* -------------------------------------------------------------------------- */
/*                                  LED Setup                                 */
/* -------------------------------------------------------------------------- */
  // strip.Begin();
  // strip.SetBrightness(40);
  // strip.ClearTo(0);
  // strip.Show();
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, PixelCount).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop() {
  renderMode();
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

/* -------------------------------------------------------------------------- */
/*                                 Animations                                 */
/* -------------------------------------------------------------------------- */
void DrawOneRainbowFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for( byte y = 0; y < MatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;      
    for( byte x = 0; x < MatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}
void movingRainbow() {
  uint32_t ms = millis();
  int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * (350 / MatrixWidth));
  int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * (310 / MatrixHeight));
  DrawOneRainbowFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
  if( ms < 5000 ) {
    FastLED.setBrightness( scale8( BRIGHTNESS, (ms * 256) / 5000));
  } else {
    FastLED.setBrightness(BRIGHTNESS);
  }
  FastLED.show();
}
void cycleRainbow() {
  uint8_t pixelHue = 0;
  EVERY_N_MILLIS(20){
    pixelHue++;
  }
  FastLED.show();
}

void movingSnake() {
  fill_solid(leds, PixelCount, CHSV(160, 255, 255));
  FastLED.show();
  EVERY_N_SECONDS(2){
    leds[random16(PixelCount)] = CHSV(64, 255, 255);
  }
}

void sparkle() {
  fadeToBlackBy( leds, PixelCount, 10);
  int pos = random16(PixelCount);
  leds[pos] += CHSV( 171 + random8(64), 200, 255);
  FastLED.show();
}

/* -------------------------------------------------------------------------- */
/*                             Animation Switcher                             */
/* -------------------------------------------------------------------------- */
// Hashing function so that we can use a switch statement on the strings
constexpr unsigned int hash(const char *s, int off = 0) {                        
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];                           
}       

void renderMode() {
  switch ( hash( current_pattern.c_str() ) ) {

/* ----------------------------------- Off ---------------------------------- */
  case hash("off"):
    FastLED.clear();
    FastLED.show();
    break;

/* --------------------------------- Rainbow -------------------------------- */
  case hash("rainbow"):
    movingRainbow();
    break;

/* ------------------------------- Solid Color ------------------------------ */
  case hash("solid"):
    fill_solid(leds, PixelCount, CHSV(192, 255, 255));
    FastLED.show();
    break;

/* --------------------------------- Sparkle -------------------------------- */
  case hash("sparkle"):
    sparkle();
    break;
  }
}