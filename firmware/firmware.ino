#include <ArduinoOTA.h>
#include <Update.h>
#include "telnet.h"
#include "grid.h"
#include "debug.h"
#include "animations/odd_face.h"
#include "animations/hexagons.h"
#include "wifi.h"
#include <EEPROM.h>

#define MAGIC_EEPROM_ADDR 0
#define MAGIC_EEPROM_NUMBER 66
#define STATE_ADDR 5

struct State {
    byte delay_ms = 44;
    byte brightness = 80;
    byte animation_index = 0;
    bool stopped = false;
};

void shutdown(unsigned int progress, unsigned int total) {
    static bool firstCall = true;
    Telnet::println("Update in progress " + String((double) progress / total * 100) + "%");

    if (firstCall) {
        LEDs::clear();
        firstCall = false;
    }
}

State state;

#define ANIMATION_COUNT 2
AnimationSequences::Sequence* animations[ANIMATION_COUNT] {
    new AnimationSequences::Hexagons,
    new AnimationSequences::OddFace
};

void saveState() {
    EEPROM.put(MAGIC_EEPROM_ADDR, (byte)MAGIC_EEPROM_NUMBER);
    EEPROM.put(STATE_ADDR, state);
    EEPROM.commit();
}

void resetAnims() {
    for (int i = 0; i < ANIMATION_COUNT; i++) {
        animations[i]->reset();
    }
}

void nextAnim() {
    if (++state.animation_index >= ANIMATION_COUNT) {
        state.animation_index = 0;
    }
}

auto handlers = new Telnet::MessageHandler[10] {
    { "anim next",      [](){ resetAnims(); nextAnim(); saveState(); LEDs::clear(); } },
    { "anim stop",      [](){ state.stopped = true; resetAnims(); saveState(); LEDs::clear(); } },
    { "anim pause",     [](){ state.stopped = true; saveState(); } },
    { "anim start",     [](){ state.stopped = false; saveState(); } },

    { "brightness inc", [](){ if(state.brightness < 250) state.brightness += 5; saveState(); LEDs::setBrightness(state.brightness); } },
    { "brightness dec", [](){ if(state.brightness > 5) state.brightness -= 5; saveState(); LEDs::setBrightness(state.brightness); } },

    { "speed inc",      [](){ if(state.delay_ms > 5) state.delay_ms -= 5; saveState(); } },
    { "speed dec",      [](){ state.delay_ms += 5; saveState(); } },

    { "reset",          [](){ esp_restart(); } },

    { "", [](){} }
};

void setup(void) {
    EEPROM.begin(128);
    setCpuFrequencyMhz(240);
    delay(200);

    LEDs::setup(state.brightness);
    Wifi::connect();

    Telnet::setup(handlers);

    ArduinoOTA.onProgress(&shutdown);
    ArduinoOTA.begin();

    LEDs::clear();

    if (EEPROM.read(MAGIC_EEPROM_ADDR) == MAGIC_EEPROM_NUMBER) {
        EEPROM.get(STATE_ADDR, state);
    }
}

void loop(void) {
    ArduinoOTA.handle();
    Telnet::loop();

    if(!state.stopped) {
        animations[state.animation_index]->step();
        LEDs::delay(state.delay_ms);
    } else {
        delay(20);
    }
}

