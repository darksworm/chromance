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

CRGB CRGBFromString(String str) {
    int firstComma = str.indexOf(',');
    int lastComma = str.lastIndexOf(',');

    byte red = str.substring(0, firstComma).toInt();
    byte green = str.substring(firstComma + 1, lastComma).toInt();
    byte blue = str.substring(lastComma + 1, str.length()).toInt();

    return CRGB(red, green, blue);
}

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

void setColor(int i, CRGB color) {
    animations[state.animation_index]->setColor(i, color);
}

auto handlers = new Telnet::MessageHandler[14] {
    { "anim next",      [](String _){ resetAnims(); nextAnim(); saveState(); LEDs::clear(); } },
    { "anim stop",      [](String _){ state.stopped = true; resetAnims(); saveState(); LEDs::clear(); } },
    { "anim pause",     [](String _){ state.stopped = true; saveState(); } },
    { "anim start",     [](String _){ state.stopped = false; saveState(); } },

    { "brightness",     [](String _){ Telnet::println("current brightnes: " + String(state.brightness)); } },
    { "brightness inc", [](String _){ if(state.brightness < 250) state.brightness += 5; saveState(); LEDs::setBrightness(state.brightness); } },
    { "brightness dec", [](String _){ if(state.brightness > 5) state.brightness -= 5; saveState(); LEDs::setBrightness(state.brightness); } },

    { "speed",          [](String _){ Telnet::println("delay between frames: " + String(state.delay_ms)); } },
    { "speed inc",      [](String _){ if(state.delay_ms > 5) state.delay_ms -= 5; saveState(); } },
    { "speed dec",      [](String _){ state.delay_ms += 5; saveState(); } },

    { "color 1 set *",  [](String color){ setColor(0, CRGBFromString(color)); } },
    { "color 2 set *",  [](String color){ setColor(1, CRGBFromString(color)); } },

    { "reset",          [](String _){ esp_restart(); } },

    { "",               [](String _){} }
};

void setup(void) {
    EEPROM.begin(128);
    setCpuFrequencyMhz(240);
    delay(200);

    if (EEPROM.read(MAGIC_EEPROM_ADDR) == MAGIC_EEPROM_NUMBER) {
        EEPROM.get(STATE_ADDR, state);
    }

    LEDs::setup(state.brightness);
    Wifi::connect();

    Telnet::setup(handlers);

    ArduinoOTA.onProgress(&shutdown);
    ArduinoOTA.begin();

    LEDs::clear();
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

