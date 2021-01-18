*Currently a work in progress*
# 16x16 LED Matrix

Codebase for my 16x16 matrix (256 pixel count) of WS2812 LEDs powered by an esp8266 (NodeMCU v1.0).

Developed with [PlatformIO](https://platformio.org/).

## Features
Heres a list of the currently implemented features.:
* Use [WifiManager](https://github.com/tzapu/WiFiManager/tree/feature_asyncwebserver) to handle Wifi Connection and credentials
* Serve static site (html, css, js) through [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
* Handle `GET` requests to change the contents of the display in realtime
* Implement basic patterns through [FastLED](https://github.com/FastLED/FastLED)

## Serving the website
The website that serves as the sole interface for this device is located in [/dev/](https://github.com/jamesmendel/16x16-Wall-Matrix/tree/master/dev). It is compiled into a header file (`full_site.h`) found in the `src` directory through the script `Convert.py`, which was modified from [ESP-HTML-Compressor](https://github.com/Gheotic/ESP-HTML-Compressor).

## Design
The full CAD can be found at [onshape.com](https://cad.onshape.com/documents/42e5a1dabde14f0667ec5ba2/w/2ec6fdbe93ad883e39ab747d/e/4e9ff106828c43007720c2b8)

The pixels are divided with laser-cut cardboard pieces.