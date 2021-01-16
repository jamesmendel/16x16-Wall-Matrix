#include <Arduino.h>
#include <ESP8266Wifi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WifiManager.h>    //branch asyncwebserver
#include <web_page.h>

#define DEBUG TRUE

WiFiManager wm;
AsyncWebServer server(80);

bool state = false;
const char* PARAM_INPUT_1 = "state";

String outputState(){
  if(state){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}

// Replaces placeholder with button section in web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    String outputStateValue = outputState();
    buttons+= "<h4>Output - GPIO 2 - State <span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

void setup() {
  
  #ifdef DEBUG  //All debug-only scripts can go here
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    // WiFi.disconnect(true);
  #endif

  wm.disconnect();
  wm.setAPStaticIPConfig(IPAddress(192,168,0,1), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
  wm.setMinimumSignalQuality(15);
  wm.setClass("invert");
  wm.autoConnect("MATRIX");  //setting broadcast ssid and password when saved fallback creds fail.
  //WifiManager halts here until a network is connected succesfully [consider wifiManager.setConfigPortalTimeout(180);]


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(LED_BUILTIN, inputMessage.toInt());
      state = !state;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(state).c_str());
  });

  // Start server
  server.begin();
  state = false;
  digitalWrite(LED_BUILTIN, state);
}


void loop() {
  digitalWrite(LED_BUILTIN, !state);
}