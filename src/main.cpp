#include <Arduino.h>
#include <ESP8266Wifi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WifiManager.h>    //branch asyncwebserver
#include <NeoPixelBrightnessBus.h>

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

NeoTopology<RowMajorAlternatingLayout> topo(MatrixWidth, MatrixHeight);
NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);
void changeMode();

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
      changeMode();
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
  strip.Begin();
  strip.SetBrightness(40);
  strip.ClearTo(0);
  strip.Show();
}

// Hashing function so that we can use a switch statement on the strings
constexpr unsigned int hash(const char *s, int off = 0) {                        
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];                           
}       

/* -------------------------------------------------------------------------- */
/*                                 Animations                                 */
/* -------------------------------------------------------------------------- */
void changeMode() {
  switch ( hash( current_pattern.c_str() ) ) {

/* ----------------------------------- Off ---------------------------------- */
  case hash("off"):
    strip.ClearTo(0);
    strip.Show();
    break;

/* -------------------------------------------------------------------------- */
/*                               Static Rainbow                               */
/* -------------------------------------------------------------------------- */
  case hash("rainbow"):
    const float c_IncrH = 1.0f / PixelCount;
    float h = 0.0f;

    for(int i=0; i<MatrixWidth; i++) {
      for(int j=0; j<MatrixHeight; j++) {
        strip.SetPixelColor(topo.Map(i, j), HslColor(h, 1.0f, 0.5f));
        h += c_IncrH;
      }
    }
    strip.Show();
    break;

/* --------------------------------- Sparkle -------------------------------- */
  case hash("sparkle"):
    //sparkle animation...
    break;
  }
}

void loop() {

}