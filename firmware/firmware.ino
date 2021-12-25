#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "credentials.h"
#include "animation.h"
#include "animation_sequence.h"
#include "grid.h"
#include "debug.h"
#include "animations/odd_face.h"
#include "animations/wifi_loading.h"
#include "animations/debug.h"

#ifndef WIFI_SSID
#pragma message "Please define WIFI_SSID"
exit(1);
#endif

#ifndef WIFI_PASS
#pragma message "Please define WIFI_PASS"
exit(1);
#endif

#define BRIGHTNESS  100
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[4][MAX_LEDS_IN_KNOT];

void connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        WifiLoading::step();
        FastLED.delay(16);
    }
}

void clearLeds() {
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < Grid::knots[k].led_count; i++) {
            leds[k][i] = CRGB::Black;
        }
    }
    FastLED.clear();
}

void setupLeds() {
    FastLED.addLeds<LED_TYPE, 32, COLOR_ORDER>(leds[0], Grid::knots[0].led_count).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, 33, COLOR_ORDER>(leds[1], Grid::knots[1].led_count).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, 25, COLOR_ORDER>(leds[2], Grid::knots[2].led_count).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, 26, COLOR_ORDER>(leds[3], Grid::knots[3].led_count).setCorrection(TypicalLEDStrip);

    FastLED.setBrightness( BRIGHTNESS );

    FastLED.clear();
    FastLED.show();
}

int getLedPosition(struct Grid::Connection connection, int index) {
    const int offset = (connection.direction == Grid::Direction::Incoming ? 13 : 0);
    return connection.strip_num * LEDS_IN_STRIP + (index * (connection.direction == Grid::Direction::Incoming ? -1 : 1)) + offset;
}

int turnOnLed(struct Grid::Connection connection, int index, CRGB color) {
    const int led_num = getLedPosition(connection, index);
    leds[(int)connection.color][led_num] = color;
}

void setup(void) {
    delay(200);
    setCpuFrequencyMhz(240);
    setupLeds();
    connectWifi();
    ArduinoOTA.begin();
    clearLeds();
}

void loop(void) {
    ArduinoOTA.handle();
    DebugAnimation::step();
    //OddFace::step();
    FastLED.delay(33);
}
