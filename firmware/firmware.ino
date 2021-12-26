#include <ArduinoOTA.h>
#include <Update.h>
#include "grid.h"
#include "debug.h"
#include "animations/odd_face.h"
#include "wifi.h"

#define BRIGHTNESS  100

void setup(void) {
    setCpuFrequencyMhz(240);
    delay(200);

    LEDs::setup(BRIGHTNESS);
    Wifi::connect();

    ArduinoOTA.begin();
    LEDs::clear();
}

void loop(void) {
    ArduinoOTA.handle();
    OddFace::step();
    LEDs::delay(33);
}
