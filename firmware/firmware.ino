#include <ArduinoOTA.h>
#include <Update.h>
#include "grid.h"
#include "debug.h"
#include "animations/odd_face.h"
#include "animations/hexagons.h"
#include "wifi.h"

#define BRIGHTNESS 80
#define DELAY 60

void shutdown(unsigned int progress, unsigned int total) {
    LEDs::darkenLeds(0.5);
    LEDs::delay(1);
}

void setup(void) {
    setCpuFrequencyMhz(240);
    delay(200);

    LEDs::setup(BRIGHTNESS);
    Wifi::connect();

    ArduinoOTA.onProgress(&shutdown);
    ArduinoOTA.begin();

    LEDs::clear();
}

void loop(void) {
    ArduinoOTA.handle();
    //HexagonsAnimation::step();
    OddFace::step();
    LEDs::delay(DELAY);
}


