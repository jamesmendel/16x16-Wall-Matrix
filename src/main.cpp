#include <Arduino.h>
#include <ESP8266Wifi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WifiManager.h>    //branch asyncwebserver

#include <LittleFS.h>
#include <full_site.h>

#define DEBUG TRUE

WiFiManager wm;
AsyncWebServer server(80);

const char* PARAM_INPUT_1 = "pattern";
String current_pattern = "off";


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
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/css", data_style_css);
    #ifdef DEBUG
      Serial.print("GET: ");
      Serial.println(data_style_css_path);
    #endif
  });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
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

/* -------------------------------------------------------------------------- */
/*                       Start server, continue to loop                       */
/* -------------------------------------------------------------------------- */

  server.begin();
}


void loop() {

}