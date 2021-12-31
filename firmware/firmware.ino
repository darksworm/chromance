#include <ArduinoOTA.h>
#include <Update.h>
#include "telnet.h"
#include "grid.h"
#include "debug.h"
#include "animations/odd_face.h"
#include "animations/hexagons.h"
#include "wifi.h"

#define BRIGHTNESS 80
#define DELAY 60

void shutdown(unsigned int progress, unsigned int total) {
    LEDs::darkenLeds(1);
    LEDs::delay(1);
    Telnet::println("Update in progress " + String(progress) + "/" + String(total) + " " + String((double) progress / total * 100) + "%");
}

bool stopped = false;
bool oddface = false;
auto handlers = new Telnet::MessageHandler[4] {
    { "anim switch", [](){ oddface = !oddface; OddFace::reset(); HexagonsAnimation::reset(); LEDs::clear(); } },
    { "anim stop", []() { stopped = true; OddFace::reset(); HexagonsAnimation::reset(); LEDs::clear(); LEDs::show(); } },
    { "anim pause", []() { stopped = true; } },
    { "anim start", []() { stopped = false; } },
};

void setup(void) {
    setCpuFrequencyMhz(240);
    delay(200);

    LEDs::setup(BRIGHTNESS);
    Wifi::connect();

    Telnet::setup(handlers);

    ArduinoOTA.onProgress(&shutdown);
    ArduinoOTA.begin();

    LEDs::clear();
}

void loop(void) {
    ArduinoOTA.handle();
    Telnet::loop();

    if(!stopped) {
        if(oddface) {
            OddFace::step();
        } else {
            HexagonsAnimation::step();
        }

        LEDs::delay(DELAY);
    } else {
        delay(20);
    }
}


